#include "http_server.h"#include "../log.h"namespace libcocao {static libcocao::Logger::ptr g_logger = LIBCOCAO_LOG_NAME("system");namespace http {    /**    * 构造函数    * @param keepalive 是否长链接    * @param worker 工作调度器    * @param io_worker    * @param accept_worker 接收连接调度器    */HttpServer::HttpServer(bool keepalive        , libcocao::IOManager* worker        , libcocao::IOManager* io_worker        , libcocao::IOManager* accept_worker)    : TcpServer(io_worker, accept_worker)    , m_isKeepalive(keepalive){    m_dispatch.reset(new ServletDispatch);    m_type = "http";}void HttpServer::setName (const std::string &v){    TcpServer::setName(v);    m_dispatch->setDefault(std::make_shared<NotFoundServlet>(v));}void HttpServer::handleClient (Socket::ptr client){    LIBCOCAO_LOG_DEBUG(g_logger) << "handleClient" << *client;    HttpSession::ptr session (new HttpSession(client));    do {        auto req = session->recvRequest();        if (!req) {            LIBCOCAO_LOG_DEBUG(g_logger) << "recv http request fail, errno="                                         << errno << " errstr=" << strerror(errno)                                         << " client:" << *client << " keep_alive=" << m_isKeepalive;            break;        }        HttpResponse::ptr rsp (new HttpResponse(req->getVersion()                                , req->isClose() || !m_isKeepalive));        rsp->setHeaders("Server", getName());        m_dispatch->handle(req, rsp, session);        session->sendResponse(rsp);        if (!m_isKeepalive || req->isClose()) {            break;        }    } while (true);    session->close();}}}