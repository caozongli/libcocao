#ifndef __LIBCOCAO_HTTP_HTTP_H__
#define __LIBCOCAO_HTTP_HTTP_H__

#include <memory>
#include <map>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <vector>
#include "http-parser/http_parser.h"

namespace libcocao {
    namespace http {
/**
 * Http方法枚举
 */
        enum class HttpMethod {
#define XX(num, name, string) name = num,
            HTTP_METHOD_MAP(XX)
#undef XX
            INVALID_METHOD
        };

/**
* HTTP状态枚举
*/
        enum class HttpStatus {
#define XX(code, name, desc) name = code,
            HTTP_STATUS_MAP(XX)
#undef XX
        };

/**
* 将字符串方法名转成HTTP方法枚举
* @param m HTTP方法
* @return HTTP方法枚举
*/
        HttpMethod StringToHttpMethod(const std::string &m);

/**
* 将字符串指针转换成HTTP方法枚举
* @param m 字符串方法枚举
* @return HTTP方法枚举
*/
        HttpMethod CharsToHttpMethod(const char *m);

/**
* 将http方法枚举转换成字符串
* @param m HTTP方法枚举
* @return 字符串
*/
        const char *HttpMethodToString(const HttpMethod &m);

/**
* 将HTTP状态枚举转换成字符串
* @param s HTTP状态枚举
* @return 字符串
*/
        const char *HttpStatusToString(const HttpStatus &s);

/**
* 忽略大小写比较仿写函数
*/
        struct CaseInsensitiveLess {
            /**
             * 忽略大小写比较字符串
             * @param lhs
             * @param rhs
             * @return
             */
            bool operator()(const std::string &lhs, const std::string &rhs) const;
        };

/**
* 获取Map中key值，并转换成对应类型，返回是否成功
* @tparam MapType
* @tparam T
* @param m Map数据结构
* @param key 关键字
* @param val 保存转换后的值
* @param def 默认值
* @return
*      true 转换成功，val为对应的值
*      false 不存在或者转换失败 val = def
*/
        template<class MapType, class T>
        bool checkGetAs(const MapType &m, const std::string &key, T &val, const T &def = T()) {
            auto it = m.find(key);
            if (it == m.end()) {
                val = del;
                return false;
            }
            try {
                val = boost::lexical_cast<T>(it->second);
                return true;
            } catch (...) {
                val = def;
            }
            return false;
        }

/**
* 获取map中key值，并转成对应类型
* @tparam MapType
* @tparam T
* @param m Map数据结构
* @param key 关键字
* @param def 默认值`
* @return 如果存在且转换成功返回对应的值，否者返回默认值
*/
        template<clss MapType, class T>
        T getAs(const MapType &m, const std::string &key, const T &def = T()) {
            auto it = m.find(key);
            if (it == m.end()) {
                return def;
            }
            try {
                return boost::lexical_cast<T>(it->second);
            } catch (...) {

            }
            return def;
        }

        class HttpResponse;

/**
* http请求结构
*/
        class HttpRequest {
        public:
            /// HTTP请求的智能指针
            typedef std::shared_ptr<HttpRequest> ptr;

            /// MAP结构
            typedef std::map<std::string, std::string, CaseInsensitiveLess> MapType;

            /**
             * 构造函数
             * @param version 版本
             * @param close 是否保持keepalive
             */
            HttpRequest(uint8_t version = 0x11, bool close = true);

            /**
             * 从HTTP请求构造HTTP响应
             * @return 只需要保证请求与响应的版本号keep-alive一致即可
             */
            std::shared_ptr<HttpResponse> createResponse();

            /**
             * @return 返回http方法
             */
            HttpMethod getMethod() const { return m_method; }

            /**
             * @return 返回HTTP版本
             */
            uint8_t getVersion() const { return m_version; }

            /**
             * 返回HTTP请求的路径
             * @return
             */
            const std::string &getPath() const { return m_path; }

            /**
             * 返回HTTP请求的查询参数
             * @return
             */
            const std::string &getQuery() const { return m_query; }

            /**
             * 返回请求的消息体
             * @return
             */
            const std::string &getBody() const { return m_body; }

            /**
             * 返回HTTP请求的消息头MAP
             * @return
             */
            const MapType &getHeaders() const { return m_headers; }

            /**
             * 返回请求消息的餐护士MAP
             * @return
             */
            const MapType &getParams() const { return m_params; }

            /**
             * 返回请求的cookie  MAP
             * @return
             */
            const MapType &getCookies() const { return m_cookies; }

            /**
             * 设置HTTP请求的方法名
             * @param v HTTP请求
             */
            void setMethod(HttpMethod v) { m_method = v; }

            /**
             * 设置HTTP请求的协议版本
             * @param v 协议版本0x11， 0x10
             */
            void setVersion(uint8_t v) { m_version = v; }

            /**
             * 设置HTTP请求的路径
             * @param v 请求路径
             */
            void setPath(const std::string &v) { m_path = v; }

            /**
             * 设置HTTP请求的查询参数
             * @param v 查询参数
             */
            void setQuery(const std::string &v) { m_query = v; }

            /**
             * 设置HTTP请求Fragment
             * @param v fragment
             */
            void setFragment(const std::string &v) { m_fragment = v; }

            /**
             * 设置HTTP请求的消息体
             * @param v
             */
            void setBody(const std::string &v) { m_body = v; }

            /**
             * 追加HTTP请求的消息体
             * @param v
             */
            void appendBody(const std::string &v) { m_body.append(v); }

            /**
             * 是否自动关闭
             * @return
             */
            bool isClose() const { return m_close; }

            /**
             * 是否websocket
             * @return
             */
            bool isWebsocket() const { return m_websocket; }

            /**
             * 设置是否为websocket
             * @param v
             * @return
             */
            void setWebsocket(bool v) { m_websocket = v; }

            /**
             * 设置HTTP请求的头部MAP
             * @param v
             */
            void setHeaders(const MapType &v) { m_headers = v; }

            /**
             * 设置HTTP请求电参数MAP
             */
            void setParams(const MapType &v) { m_path = v; }

            /**
             * 设置HTTP请求的Cookies MAP
             * @param v  map
             */
            void setCookies(const MapType &v) { m_cookies = v; }

            /**
             * 获取HTTP请求的头部参数
             * @param key 关键字
             * @param def 默认值
             * @return 如果存在则返回对应值，否则返回默认值
             */
            std::string getHeader(const std::string &key, const std::string &def = "") const;

            /**
             * 获取HTTP请求请求的请求参数
             * @param key 关键字
             * @param def 默认值
             * @return 如果存在则返回对应值，否则返回默认值
             */
            std::string getParam(const std::string &key, const std::string &def = "");

            /**
             * 获取HTTP请求中Cookie参数
             * @param key 关键字
             * @param def 默认值
             * @return 如果存在则返回对应值，否则返回默认值
             */
            std::string getCookie(const std::string &key, const std::string &def = "");

            /**
             * 设置HTTP请求中的头部参数
             * @param key 关键字
             * @param val 值
             */
            void stdHeader(const std::string &key, const std::string &val);

            /**
             * 设置http请求中请求参数
             * @param key 关键字
             * @param val 值
             */
            void setParam(const std::string &key, const std::string &val);

            /**
             * 设置HTTP请求的Cookie参数
             * @param key 关键字
             * @param val 值
             */
            void setCookie(const std::string &key, const std::string &val);

            /**
             * 删除请求的头部参数
             * @param key
             */
            void delHeader(const std::string &key);

            /**
             * 删除HTTP请求的请求参数
             * @param key 关键字
             */
            void delParam(const std::string &key);

            /**
             * 删除HTTP请求的cookie参数
             */
            void delCookie(const std::string &key);

            /**
             * 判断HTTP请求的头部参数是否存在
             * @param key 关键字
             * @param val 如果存在，val非空则赋值
             * @return
             */
            bool hasHeader(const std::string &key, std::string *val = nullptr);

            /**
             * 判断HTTP请求的请求参数是否存在
             * @param key 关键字
             * @param val 如果存在，val非空则赋值
             */
            bool hasParam(const std::string &key, std::string *val = nullptr);

            /**
             * 判断HTTP请求的cookie参数是否存在
             * @param key 关键字
             * @param val 如果存在，val非空则赋值
             * @return 是否存在
             */
            bool hasCookie(const std::string &key, std::string &val = nullptr);

            /**
             * 检查并获取HTTP请求的头部参数
             * @tparam T 转换类型
             * @param key 关键字
             * @param val 返回值
             * @param def 默认值
             * @return 如果存在且转换成功返回true，否则失败val=def
             */
            template<class T>
            bool checkGetHeaderAs(const std::string &key, Y &val, const T &def = T()) {
                return checkGetAs(m_headers, key, val, def);
            }

            /**
             * 获取HTTP请求的头部参数
             * @tparam T 转换类型
             * @param key 关键字
             * @param def 默认值
             * @return 如果存在且转换成功返回对应的值，否则返回def
             */
            template<class T>
            T getHeaderAs(const std::string &key, const T &def = T()) {
                return getAs(m_headers, key, def);
            }

            /**
             * 检查并获取HTTP请求的请求的参数
             * @tparam T 转换类型
             * @param key 关键字
             * @param val 返回值
             * @param def 默认值
             * @return 如果存在且转换成功返回true，否则失败val=def
             */
            template<class T>
            bool checkGetParamAs(const std::string &key, T &val, const T &def = T()) {
                initQueryParam();
                initBodyParam();
                return checkGetAs(m_params, key, val, def);
            }

            /**
             * 获取HTTP请求的请求参数
             * @param T 转换类型
             * @param key 关键字
             * @param def 默认值
             * @return 如果存在且转换成功返回对应的值，否则返回def
             */
            template<class T>
            T getParamAs(const std::string &key, const T &def = T()) {
                initQueryParam();
                initBodyParam();
                return getAs(m_params, key, def);
            }

            /**
             * 检查并获取HTTP请求请求的Cookie参数
             * @tparam T 转换类型
             * @param key 关键字
             * @param val 返回值`
             * @param def 默认值
             * @return 如果存在且转换成功返回true，否则失败val = def
             */
            template<class T>
            bool checkGetCookieAs(const std::string &key, T &val, const T &def = T()) {
                initCookies();
                return checkGetAs(m_cookies, key, val, def);
            }

            /**
             * 获取HTTP请求的Cookie参数
             * @tparam T 转换类型
             * @param key 关键字
             * @param def 默认值
             * @return 如果存在且转换成功返回对应的值，否则返回def
             */
            template<class T>
            T getCookieAs(const std::string &key, const T &def = T()) {
                initCookies();
                return getAs(m_cookies, key, def);
            }

            /**
             * 序列化输出到流中
             * @param os 输出流
             * @return 输出流
             */
            std::ostream &dump(std::ostream &os) const;

            /**
             * 转换成字符串类型
             * @return 字符串
             */
            std::string toString() const;

            /**
             * 提取url中的查询参数
             */
            void initQueryParam();

            /**
             * 当content-type是application/x-www-form-urlencoded时，提取消息中的表单参数
             */
            void initBodyParam();

            /**
             * 提取请求中的cookies
             */
            void initCookies();

            /**
             * 初始化，实际是判断connection是否为keep-alive，以设置是否自动关闭套接字
             */
            void init();

        private:
            /// HTTP方法
            HttpMethod m_method;
            /// HTTP版本
            uint8_t m_version;
            ///是否自动关闭
            bool m_close;
            /// 是否为websocket
            bool m_websocket;
            /// 参数解析标志位，0：未解析，1：已解析url参数，2：已解析http消息体中的参数，4：已解析cookies
            uint8_t m_parserParamFlag;
            /// 请求的完整url
            std::string m_url;
            /// 请求路径
            std::string m_path;
            /// 请求参数
            std::string m_query;
            /// 请求fragment
            std::string m_fragment;
            /// 请求消息体
            std::string m_body;
            /// 请求头部MAP
            MapType m_headers;
            /// 请求参数MAP
            MapType m_params;
            /// 请求Cookie MAP
            MapType m_cookies;
        };

        class HttpResponse {
        public:
            /// HTTP响应结构只能指针
            typedef std::shared_ptr<HttpResponse> ptr;

            ///MapType
            typedef std::map<std::string, std::string, CaseInsensitiveLess> MapType;

            /**
             * 构造函数
             * @param version 版本
             * @param close 是否自动关闭
             */
            HttpResponse(uint8_t version = 0x11, bool close = true);

            /**
             * 返回响应状态
             * @return 版本
             */
            HttpStatus getStaus() const { return m_status; }

            /**
             * 返回响应版本
             * @return 版本
             */
            uint8_t getVersion() const { return m_version; }

            /**
             * 返回响应消息体
             * @return 消息体
             */
            const std::string &getBody() const { return m_body; }

            /**
             * 返回响应原因
             * @return
             */
            const std::string &getReason() const { return m_reason; }

            /**
             * 返回响应头部MAP
             * @return
             */
            const MapType &getHeaders() const { return m_headers; }

            /**
             * 设置响应状态
             * @param v 响应状态
             */
            void getStatus(HttpStatus v) { m_status = v; }

            void setStatus(HttpStatus v) { m_status = v; }

            void setVersion (uint8_t v) { m_version = v; }

            /**
             * 追加HTTP请求的消息体
             * @param v 追加内容
             */
            void setBody(const std::string &v) { m_body = v; }

            /**
             * 设置响应原因
             * @param v 原因
             */
            void appendBody(const std::string &v) { m_reason = v; }

            /**
             * 设置响应头部MAP
             * @param v MAP
             */
            void setHeaders(const MapType &v) { m_headers = v; }

            /**
             * 设置是否自动关闭
             * @param v
             * @return
             */
            bool isClose(bool v) { m_close = v; }

            /**
             * 是否websocket
             * @return
             */
            bool isWebsocket() const { return m_websocket; }

            /**
             * 获取响应头部参数
             * @param key 关键字
             * @param def 默认值
             * @return 如果存在返回对应值, 否则返回def
             */
            std::string getHeader(const std::string &key, const std::string &def = "") const;

            /**
             * 设置响应头部参数
             * @param key 关键字
             * @param val 值
             */
            void setHeaders(const std::string &key, const std::string &val);

            /**
             * 删除响应头部参数
             * @param key
             */
            void delHeader(const std::string &key);

            /**
             * 检查并相应头部参数
             * @tparam T 值类型
             * @param key 关键字
             * @param val 值
             * @param def 默认值
             * @return 如果存在且转换成功返回true，否则失败val=def
             */
            template<class T>
            bool checkGetHeaderAs(const std::string &key, T &val, const T &def = T()) {
                return checkGetAs(m_headers, key, val, def);
            }

            /**
             * 获取响应的头部参数
             * @tparam T 转换类型
             * @param key 关键字
             * @param def 默认值
             * @return 如果存在且转换成功返回对应的值，否则返回def
             */
            template<class T>
            T getHeaderAs(const std::string &key, const T &def = T()) {
                return getAs(m_headers, key, def);
            }

            /**
             * 序列化输出到流
             * @param os 输出流
             * @return 输出流
             */
            std::ostream &dump(std::ostream &os) const;

            /**
             * 转换成字符串
             * @return
             */
            std::string toString() const;

            /**
             * 设置重定向，在头部添加Location字段，值为uri
             * @param uri 目标uri
             */
            void setRedirect(const std::string &uri);

            /**
             * 为响应添加cookie
             * @param[] key cookie的key之
             * @param[] val cookie的value
             * @param[] expired 过期时间
             * @param[] path cookie的影响路径
             * @param[] domain cookie作用的域
             * @param[] secure 安全标志
             */
            void setCookie(const std::string &key, const std::string &val,
                           time_t expired = 0,,

            const std::string &path = "",
            const std::string &domain = "",
            bool secure = false
            );

        private:
            /// 响应状态
            HttpStatus m_status;
            /// 版本
            uint8_t m_version;
            /// 是否自动关闭
            bool m_close;
            /// 是否为websocket
            bool m_websocket;
            /// 响应消息体
            std::string m_body;
            /// 响应原因
            std::string m_reason;
            /// 响应头部MAP
            MapType m_headers;
            /// cookies
            std::vector<std::string> m_cookies;

        };
    }
}
#endif