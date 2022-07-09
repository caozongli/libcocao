#include "tcp_server.h"
#include "log.h"

namespace libcocao {

static Logger::ptr g_logger = LIBCOCAO_LOG_NAME("system");

/**
   * 构造函数
   * @param name 服务器名称
   * #param type 服务器类型
   * @param io_worket socket客户端工作的协程调度器
   * @param accept_worker 服务器socket执行接收socket连接的协程调度器
   */
TcpServer::TcpServer (libcocao::IOManager* io_worker
                        , libcocao::IOManager *accept_worker)
    : m_ioworker (io_worker)
    , m_acceptWorker (accept_worker)
    , m_recvTimeout (5000)
    , m_name ("libcocao/1.0.1")
    , m_type ("tcp")
    , m_isStop (true) {

}

/**
 * 析构函数
 */
TcpServer::~TcpServer() {
    for (auto &i :m_socks)
        i->close();
    m_socks.clear();
}

/**
 * 绑定地址
 * @param addr
 * @return 返回是否绑定成功
 */
bool TcpServer::bind (libcocao::Address::ptr addr){
    std::vector<Address::ptr> addrs;
    std::vector<Address::ptr> fails;
    addrs.push_back (addr);
    return bind (addrs, fails);
}

/**
 * 绑定地址数组
 * @param addr 需要绑定的地址数组
 * @param fails 绑定失败的地址
 * @return 是否绑定成功
 */
bool TcpServer::bind (const std::vector<Address::ptr>& addrs
        , std::vector<Address::ptr>& fails){
    for (auto &addr : addrs) {
        Socket::ptr sock = Socket::CreateTCP(addr);
        if (!sock->bind(addr)) {
            LIBCOCAO_LOG_ERROR(g_logger) << "bind fail errno="
                                         << errno << " errstr=" << strerror(errno)
                                         << " addr=[" << addr->toString() << "]";
            fails.push_back(addr);
            continue;
        }
        if (!sock->listen()) {
            LIBCOCAO_LOG_ERROR(g_logger) << "listen fail errno="
                                         << errno << " errstr=" << strerror(errno)
                                         << " addr=[" << addr->toString() << "]";
            fails.push_back(addr);
            continue;
        }
    }
    if (!fails.empty()) {
        m_socks.clear();
        return false;
    }

    for (auto & i : m_socks) {
        LIBCOCAO_LOG_INFO(g_logger) << "type=" << m_type
            << " name=" << m_name
            << " server bind success: " << *i;
    }
    return true;
}

/**
 * 启动服务
 * @return 需要bind成功后执行
 */
bool TcpServer::start (){
    if (!m_isStop)
        return true;
    m_isStop = false;
    for (auto & sock: m_socks) {
        m_acceptWorker->schedule(std::bind(&TcpServer::startAccept, shared_from_this(), sock));
    }
    return true;
}

/**
 * 停止服务
 */
void TcpServer::stop(){
    m_isStop = true;
    auto self = shared_from_this();
    m_acceptWorker->schedule([this, self]() {
        for (auto& sock : m_socks) {
            sock->cancelAll();
            sock->close();
        }
        m_socks.clear();
    });
}

/**
 * 以字符串形式dump server信息
 * @param prefix
 * @return
 */
std::string TcpServer::toString (const std::string &prefix){
    std::stringstream ss;
    ss << prefix << "[type=" << m_type
        << " name=" << m_name
        << " io_worker=" << (m_ioworker ? m_ioworker->getName() : "")
        << "accpet=" << (m_acceptWorker ? m_acceptWorker->getName() : "")
        << " recv_timeout=" << m_recvTimeout << "]" << std::endl;
    std::string pfx = prefix.empty() ? "    " : prefix;
    for (auto &i : m_socks) {
        ss << pfx << pfx << *i << std::endl;
    }
    return ss.str();
}

/**
 * 处理新连接的Socket类
 * @param client
 */
void TcpServer::handleClient (Socket::ptr client){
    LIBCOCAO_LOG_INFO(g_logger) << "handleClient: " << *client;
}

/**
 * 开始接受连接
 * @param sock
 */
void TcpServer::startAccept (Socket::ptr sock){
    while (!m_isStop) {
        Socket::ptr client = sock->accept();
        if (client) {
            client->setRecvTimeout(m_recvTimeout);
            m_ioworker->schedule(std::bind(&TcpServer::handleClient,
                                           shared_from_this(), client));
        } else {
            LIBCOCAO_LOG_ERROR(g_logger) << "accept errno=" << errno
                << " errstr=" << strerror(errno);
        }
    }
}


}