#ifndef __LIBCOCAO_SOCKET_STREAM_H__
#define __LIBCOCAO_SOCKET_STREAM_H__

#include "../stream.h"
#include "../socket.h"
#include "../mutex.h"
#include "../iomanager.h"

namespace libcocao {
class SocketStream : public Stream {
public:
    typedef std::shared_ptr<SocketStream> ptr;

    /**
     * 构造函数
     * @param sock Socket类
     * @param owner 是否完全控制
     */
    SocketStream (Socket::ptr sock, bool owner = true);

    /**
     * 析构函数
     */
    ~SocketStream();

    /**
     * 读取数据
     * @param buffer 待接收数据的内存
     * @param length 待接收数据的内存长度
     * @return
     *      > 0 返回实际接收到的数据长度
     *      = 0 socket被远端关闭
     *      < 0 socket错误
     */
    virtual int read (void *buffer, size_t length) override;

    /**
     * 读取数据2
     * @param ba 接受数据的ByteArray
     * @param length 待接收数据的内存长度
     * @return
     *      > 0 返回实际接受到的数据长度
     *      = 0 socket被远端关闭
     *      < 0 socket错误
     */
    virtual int read (ByteArray::ptr ba, size_t length) override;

    /**
     * 写入数据
     * @param buffer 待发送数据的内存
     * @param length 待发送数据的内存长度
     * @return
     *      > 0 返回实际发送的数据长度
     *      = 0 socket别远端关闭
     *      < o socket错误
     */
    virtual int write (const void *buffer, size_t length) override;

    /**
     * @param ba
     * @param length
     * @return
     */
    virtual int write (ByteArray::ptr ba, size_t length) override;

    /**
     * 关闭socket
     */
    virtual void close () override;

    /**
     * 返回socket类
     * @return
     */
    Socket::ptr getSocket () const { return m_socket; }

    /**
     * 返回是否连接
     * @return
     */
    bool isConnected() const;

    /**
     * 获取远端地址
     * @return
     */
    Address::ptr getRemoteAddress ();

    /**
     *获取本地地址
     * @return
     */
    Address::ptr getLocalAddress ();

    /**
     *
     * @return
     */
    std::string getRemoteAddressString ();

    /**
     *
     * @return
     */
    std::string getLocalAddressString ();


private:
    /// socket类
    Socket::ptr m_socket;
    /// 是否主控
    bool m_owner;

};

}

#endif