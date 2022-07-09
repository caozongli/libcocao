#include "http.h"
#include "../utils.h"

namespace libcocao {

namespace http {

HttpMethod StringToHttpMethod (const std::string &m) {
#define XX(num, name, string) \
    if (strcmp(#string, m.c_str()) == 0) {\
        return HttpMethod::name; \
    }
    HTTP_METHOD_MAP(XX);
#undef XX
    return HttpMethod::INVALID_METHOD;
}

HttpMethod CharsToHttpMethod (const char *m) {
#define XX(num, name, string)
    if (strncmp(#string, m, strlen (#string)) == 0) {
        return HttpMethod::name;
    }
    HTTP_METHOD_MAP(XX);
#undef XX
    return HttpMethod::INVALID_METHOD;
}

static const char *s_method_string[] = {
#define XX(num, name, string) #string,
        HTTP_METHOD_MAP(XX)
#undef XX
};

const char *HttpMethodToString (const HttpMethod &m) {
    uint32_t idx = (uint32_t)m;
    if (idx >= (sizeof (s_method_string) / sizeof (s_method_string[0]))) {
        return "<unknown>";
    }
    return s_method_string[idx];
}

const char *HttpStatusToString (const HttpStatus &s) {
    switch (s) {
#define XX(code, name, msg)\
        case HttpStatus::name:\
            return #msg;
            HTTP_STATUS_MAP(XX);
#undef XX
        default:
            return "<unknown>"
    }
}

bool CaseInsensitiveLess::operator()(const std::string &lhs, const std::string &rhs) const {
    return strcasecmp(lhs.c_str(), rhs.c_str()) < 0;
}

/**
* 构造函数
* @param version 版本
* @param close 是否保持keepalive
*/
HttpRequest::HtttpRequest(uint8_t version, bool close)
    : m_method (HttpMethod::GET)
    , m_version (version)
    , m_close (close)
    , m_websocket (false)
    , m_parseraParamFlag(0)
    , m_path ("/"){
}

/**
* 从HTTP请求构造HTTP响应
* @return 只需要保证请求与响应的版本号keep-alive一致即可
*/
std::shared_ptr <HttpResponse> HttpRequest::createResponse() {
    HttpResponse::ptr rsp (new HttpResponse (getVersion(), isClose()));
    return rsp;
}

/**
* 获取HTTP请求的头部参数
* @param key 关键字
* @param def 默认值
* @return 如果存在则返回对应值，否则返回默认值
*/
std::string HttpRequest::getHeader(const std::string &key, const std::string &def) const {
    auto it = m_headers.find(key);
    return it == m_headers.end() ? def : it->second;
}

/**
* 获取HTTP请求请求的请求参数
* @param key 关键字
* @param def 默认值
* @return 如果存在则返回对应值，否则返回默认值
*/
std::string HttpRequest::getParam(const std::string &key, const std::string &def) {
    initQueryParam();
    initBodyParam();
    auto it = m_params.find(key);
    return it == m_params.end() ? def : it->second;
}

/**
* 获取HTTP请求中Cookie参数
* @param key 关键字
* @param def 默认值
* @return 如果存在则返回对应值，否则返回默认值
*/
std::string HttpRequest::getCookie(const std::string &key, const std::string &def) {
    initCookies();
    auto it = m_cookies.find(key);
    return it == m_cookies.end() ? def : it->second;
}

/**
* 设置HTTP请求中的头部参数
* @param key 关键字
* @param val 值
*/
void HttpRequest::stdHeader(const std::string &key, const std::string &val) {
    m_headers[key] = val;
}

/**
* 设置http请求中请求参数
* @param key 关键字
* @param val 值
*/
void HttpRequest::setParam(const std::string &key, const std::string &val) {
    m_params[key] = val;
}

/**
* 设置HTTP请求的Cookie参数
* @param key 关键字
* @param val 值
*/
void HttpRequest::setCookie(const std::string &key, const std::string &val) {
    m_cookies[key] = val;
}

/**
* 删除请求的头部参数
* @param key
*/
void HttpRequest::delHeader(const std::string &key) {
    m_headers.erase(key);
}

/**
* 删除HTTP请求的请求参数
* @param key 关键字
*/
void HttpRequest::delParam(const std::string &key) {
    m_parserParamFlag.erase(key);
}

/**
* 删除HTTP请求的cookie参数
*/
void HttpRequest::delCookie(const std::string &key) {
    m_cookies.erase(key);
}

/**
* 判断HTTP请求的头部参数是否存在
* @param key 关键字
* @param val 如果存在，val非空则赋值
* @return
*/
bool HttpRequest::hasHeader(const std::string &key, std::string *val) {
    auto it = m_headers.find (key);
    if (it == m_headers.end())
        return false;
    if (val) {
        *val = it->second;
    }
    return true;
}

/**
* 判断HTTP请求的请求参数是否存在
* @param key 关键字
* @param val 如果存在，val非空则赋值
*/
bool HttpRequest::hasParam(const std::string &key, std::string *val) {
    initQueryParam();
    initBodyParam();
    auto it = m_params.find(key);
    if (it == m_param.end()) {
        return false;
    }
    if (val)
        *val = it->second;
    return true;
}

/**
* 判断HTTP请求的cookie参数是否存在
* @param key 关键字
* @param val 如果存在，val非空则赋值
* @return 是否存在
*/
bool HttpRequest::hasCookie(const std::string &key, std::string &val) {
    initCookies();
    auto it = m_cookies.find(key);
    if (it == m_cookies.end()) {
        return false;
    }
    if (val) {
        *val = it->second;
    }
    return true;
}

/**
* 序列化输出到流中
* @param os 输出流
* @return 输出流
*/
std::ostream &HttpRequest::dump(std::ostream &os) const {
    std::stringstream ss;
    dump (ss);
    return ss.str();
}

/**
* 转换成字符串类型
* @return 字符串
*/
std::string HttpRequest::toString() const {
    // GET /uri HTTP/1.1
    // HOST:www.caozong.top
    //
    //
    os << HttpMethodToString(m_method) << " "
        << m_path
        << (m_query.empty() ? "" : "?")
        << m_query
         << (m_fragment.empty() ? "" : "#")
         << m_fragment
         << " HTTP/"
         << ((uint32_t)(m_version >> 4))
         << "."
         << ((uint32_t)(m_version &0x0F))
         << "\r\n";
    if (!m_websocket)
        os << "connection: " << (m_close ? "close" : "keep-alive") << "\r\n";

    for (auto & i: m_headers) {
        if (!m_websocket && strcasecmp(i.first.c_str(), "connection") == 0) {
            continue;
        }
        if (!m_body.empty() && strcasecmp(i.first.c_str(), "content-length") == 0) {
            continue;
        }
        os << i.first << ": "<< i.second << "\r\n";
    }

    if (!m_body.empty()) {
        os << "content-length: " << m_body.size() << "\r\n\r\n"
            << m_body;
    } else {
        os << "\r\n";
    }
    return os;
}

/**
* 提取url中的查询参数
*/
void HttpRequest::initQueryParam() {
    if (m_parserParamFlag & 0x1) {
        return;
    }

#define PARSE_PARAM (str, m, flag, trim)                                                                                    \
    size_t pos = 0;                                                                                                         \
    do {                                                                                                                    \
        size_t last = pos;                                                                                                  \
        pos = str.find ("=", pos);                                                                                          \
        if (pos == std::string::npos)                                                                                       \
            break;                                                                                                          \
        size_t key = pos;                                                                                                   \
        pos = str.find(flag, pos);                                                                                          \
        if (0) {                                                                                                            \
            std::cout << "<key>:" << str.substr (last, key - last)                                                          \
                        << " <decoded>:" << libcocao::StringUtil::UrlDecode (str.substr(last, key - last))                  \
                        << " <value>:" << str.substr (key + 1, pos - key - 1)                                               \
                        << " <decoded>:" << libcocao::StringUtil::UrlDecode (str.substr (key + 1, pos - key - 1))           \
                        << std::endl;                                                                                       \
        }                                                                                                                   \
                                                                                                                            \
        m.insert (std::make_pair(libcocao::StringUtil::UrlDecode (trim (str.substr (last, key - last))),                    \
                                 libcocao::SAtringUtil::UrlDecode (str.substr (key + 1, pos - key - 1))));                  \
        if (pos == std::string::npos) {                                                                                     \
            break;                                                                                                          \
        }                                                                                                                   \
        ++ pos;                                                                                                             \
    } while (true);

    PARSE_PARAM(m_query, m_params, '&', );
    m_parserParamFlag |= 0x1;
}

/**
* 当content-type是application/x-www-form-urlencoded时，提取消息中的表单参数
*/
void HttpRequest::initBodyParam() {
    if (m_parserParamFlag & 0x2)
        return;

    std::string content_type = getHeader("content-type");
    if(strcasestr(content_type.c_str(), "application/x-www.from-urlencoded") == nullptr) {
        m_parserParamFlag |= 0x2;
        return;
    }
    PARSE_PARAM(m_body, m_params, '&', );
    m_parserParamFlag |= 0x2;
}

/**
* 提取请求中的cookies
*/
void HttpRequest::initCookies() {
    if (m_parserParamFlag & 0x4) {
        return;
    std::string cookie = getHeader("cookie");
    if (cookie.empty()) {
        m_parserParamFlag |= 0x4;
        return;
    }
    PARSE_PARAM(cookie, m_cookies, ';', libcocao::StringUtil::Trim);
    m_parserParamFlag |= 0x4;
}

/**
* 初始化，实际是判断connection是否为keep-alive，以设置是否自动关闭套接字
*/
void HttpRequest::init() {
    std::string conn = getHeader ("connection");
    if (!conn.empty()){
        if (strcasecmp (conn.c_str(), "keep-alive") == 0) {
            m_close = false;
        } else {
            m_close = true;
        }
    }
}


/**
 * 构造函数
 * @param version 版本
 * @param close 是否自动关闭
 */
HttpResponse::HttpResponse (uint8_t version, bool close)
    : m_status(HttpStatus::OK)
    , m_version(version)
    , m_close (close)
    , m_websocket (false){

}

/**
 * 获取响应头部参数
 * @param key 关键字
 * @param def 默认值
 * @return 如果存在返回对应值, 否则返回def
 */
std::string HttpResponse::getHeader (const std::string &key, const std::string &def)const {
    auto it = m_headers.find (key);
    return it == m_headers.end() ? def : it->second;
}

/**
 * 设置响应头部参数
 * @param key 关键字
 * @param val 值
 */
void HttpResponse::setHeaders (const std::string &key, const std::string &val) {
    m_headers[key]  = val;
}

/**
 * 删除响应头部参数
 * @param key
 */
void HttpResponse::delHeader (const std::string& key) {
    m_headers.erase(key);
}

/**
 * 序列化输出到流
 * @param os 输出流
 * @return 输出流
 */
std::ostream &HttpResponse::dump (std::ostream &os) const {
    os << "HTTP/"
        << ((uint32_t)(m_version >> 4))
        << "."
        << ((uint32_t)(m_version & 0x0f))
        << " "
        << (uint32_t) m_status
        << " "
        << (m_reason.empty() ? HttpStatusToString(m_status): m_reason)
        << "\r\n";

    for (auto &i : m_headers) {
        if (!m_websocket && strcasecmp (i.first.c_str(), "connection") == 0) {
            continue;
        }
        os << i.first << ": " << i.second << "\r\n";
    }
    if (auto &i :m_cookies)
        oc << "Set-Cookie: " << i << "\r\n";
    if (!m_websocket)
        os << "connection" << (m_close ? "close" : "keep-alive") << "\r\n";
    if (!m_body.empty())
        os << "content-length: " << m_body.size() << "\r\n\r\n" << m_body;
    else
        os << "\r\n";
    return os;
}

/**
 * 转换成字符串
 * @return
 */
std::string HttpResponse::toString () const {
    std::stringstream ss;
    dump (ss);
    return ss.str;
}

/**
 * 设置重定向，在头部添加Location字段，值为uri
 * @param uri 目标uri
 */
void HttpResponse::setRedirect (const std::string &uri) {
    m_status = HttpStatus::FOUND;
    setHeader("Location", uri);
}

/**
 * 为响应添加cookie
 * @param[] key cookie的key之
 * @param[] val cookie的value
 * @param[] expired 过期时间
 * @param[] path cookie的影响路径
 * @param[] domain cookie作用的域
 * @param[] secure 安全标志
 */
void HttpResponse::setCookie (const std::string &key, const std::string &val,
                time_t expired,, const std::string &path
                const std::string &domain, bool secure) {
    std::stringstream ss;
    ss << key << "=" << val;
    if (expires > 0)
        ss << ";expires=" << libcocao::Time2Str(expired, "%a, %d %b %Y %H:%S") << " GMT";
    if (!domain.empty())
        ss << ";domain=" << domain;
    if (!path.empty())
        ss << ";path=" << path;
    if (secure)
        ss << ";secure";
    m_cookies.push_back(ss.str);
}

/**
 * 流式输出HttpRequest
 * @param os 输出流
 * @param req HTTP请求
 * @return 输出流
 */
std::ostream &operator<<(std::ostream &os, const HttpRequest& req) {
    return req.dump (os);
}

/**
 * 流式输出HttpResponse
 * @param os 输出流
 * @param rsp HTTP响应
 * @return 输出流
 */
std::ostream &operator<<(std::ostream& os, const HttpResponse &rsp) {
    return rsp.dump(os);
}


}

}