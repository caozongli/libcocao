#include "socket_stream.h"

namespace libcocao {

/**
    * 构造函数
    * @param sock Socket类
    * @param owner 是否完全控制
    */
SocketStream::SocketStream (Socket::ptr sock, bool owner)
    : m_socket(sock)
    , m_owner (owner){

}

/**
 * 析构函数
 */
SocketStream::~SocketStream(){
    if (m_owner && m_socket) {
        m_socket->close();
    }
}

/**
 * 读取数据
 * @param buffer 待接收数据的内存
 * @param length 待接收数据的内存长度
 * @return
 *      > 0 返回实际接收到的数据长度
 *      = 0 socket被远端关闭
 *      < 0 socket错误
 */
int SocketStream::read (void *buffer, size_t length) {
    if (!isConnected()) return -1;
    return m_socket->recv(buffer, length);
}

/**
 * 读取数据2
 * @param ba 接受数据的ByteArray
 * @param length 待接收数据的内存长度
 * @return
 *      > 0 返回实际接受到的数据长度
 *      = 0 socket被远端关闭
 *      < 0 socket错误
 */
int SocketStream::read (ByteArray::ptr ba, size_t length) {
    if (!isConnected()) return -1;
    std::vector<iovec> iovs;
    ba->getWriteBUffers(iovs, length);
    int rt = m_socket->recv(&iovs[0], iovs.size());
    if (rt > 0) {
        ba->setPosition(ba->getPosition() + rt);
    }
    return rt;
}

/**
 * 写入数据
 * @param buffer 待发送数据的内存
 * @param length 待发送数据的内存长度
 * @return
 *      > 0 返回实际发送的数据长度
 *      = 0 socket别远端关闭
 *      < o socket错误
 */
int SocketStream::write (const void *buffer, size_t length) {
    if (!isConnected()) return -1;
    return m_socket->send(buffer, length);
}

/**
 * @param ba
 * @param length
 * @return
 */
int SocketStream::write (ByteArray::ptr ba, size_t length) {
    if (!isConnected()) return -1;

    std::vector<iovec> iovs;
    ba->getWriteBUffers(iovs, length);
    int rt = m_socket->send(&iovs[0], iovs.size());
}

/**
 * 关闭socket
 */
void SocketStream::close () {
    if (m_socket)
        m_socket->close();
}

/**
 * 返回是否连接
 * @return
 */
bool SocketStream::isConnected() const{
    return m_socket && m_socket->isConnected();
}

/**
 * 获取远端地址
 * @return
 */
Address::ptr SocketStream::getRemoteAddress (){
    if (m_socket) {
        return m_socket->getRemoteAddress();
    }
    return nullptr;
}

Address::ptr SocketStream::getLocalAddress (){
    if (m_socket) {
        return m_socket->getLocalAddress();
    }
    return nullptr;
}

std::string SocketStream::getRemoteAddressString (){
    auto addr = getRemoteAddress();
    if (addr) {
        return addr->toString();
    }
    return "";
}

std::string SocketStream::getLocalAddressString (){
    auto addr = getLocalAddress();
    if (addr) {
        return addr->toString();
    }
    return "";
}

}