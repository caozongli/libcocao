#ifndef __LIBCOCAO_HTTP_SESSION_H__#define __LIBCOCAO_HTTP_SESSION_H__#include "../streams/socket_stream.h"#include "http.h"namespace libcocao {namespace http {class HttpSession : public SocketStream {public:    typedef std::shared_ptr<HttpSession> ptr;    /**     * 构造函数     * @param sock Socket类型     * @param owner 是否托管     */    HttpSession (Socket::ptr sock, bool owner = true);    /**     * 接收HTTP请求     * @return     */    HttpRequest::ptr recvRequest();    /**     * 发送HTTP响应     * @param rsp HTTP响应     * @return     *      > 0 发送成功     *      = 0 对方关闭     *      < 0 Socket异常     */    int sendResponse (HttpResponse::ptr rsp);};}}#endif