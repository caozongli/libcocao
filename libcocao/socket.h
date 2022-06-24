#define __LIBCOCAO_SOCKET_H__
#define __LIBCOCAO_SOCKET_H__

#include <memory>
#include "noncopyable.h"
#include "address.h"

namespace libcocao {
class Socket : public std::enable_shared_from_this<Socket>, Noncopyable {
public:
    typedef std::shared_ptr<Socket> ptr;
    typedef std::weak_ptr<Socket> weak_ptr;

    /**
     * Socket类型
     */
    enum Type {
        /// TCP类型
        TCP = SOCK_STREAM,
        /// UDP类型
        UDP = SOCK_DGRAM
    };

    /**
     * Socket协议族
     */
     enum Family {
         /// IPv4 socket
         IPv4 = AF_INET,
         /// IPv6 socket
         IPv6 = AF_INET6,
         /// Unix socket
         UNIX = AF_UNIX
     };

    /**
    * 创建TCP Socket(满足地址类型)
    * @param address 地址
    * @return
    */
    static Socket::ptr CreateTCP (libcocao::Address::ptr address);

    /**
    * 创建UDP Socket（满足地址类型）
    */
    static Socket::ptr CreateUDP (libcocao::Address::ptr address);

    /**
    * 创建IPv4的TCP Socket
    */
    static Socket::ptr CreateTCPSocket ();

    /**
    * 创建IPv4的UDP Socket
    * @return
    */
    static Socket::ptr CreateUDPSocket ();

    /**
    * 创建IPv6的TCP Socket
    */
    static Socket::ptr CreateTCPSocket6 ();

    /**
    * 创建IPv6 的UDP Socket
    */
    static Socket::ptr CreateUDPSocket6 ();

    /**
    * 创建Unix的TCP Socket
    */
    static Socket::ptr CreateUnixTCPSocket ();

    /**
    * 创建Unix的UDP Socket
    */
    static Socket::ptr CreateUnixUDPSocket ();

    /**
    * Socket构造函数
    * family 协议族
    * type 类型
    * protocol 协议
    */
    Socket (int family, int type, int protocol = 0);

    /**
    * 析构函数
    */
    virtual ~Socket();

    /**
    * 获取发送超时时间(毫秒）
    * @return
    */
    int getSendTimeout ();

    /**
    * 设置发送超时时间毫秒
    * @param v
    */
    void setSendTimeout (int64_t v);

    /**
     * 获取接受超时时间（毫秒）
     * @return
     */
    int64_t getRecvTimeout ();

    /**
     * 设置接受超时时间（毫秒）
     * @param v
     */
    void setRecvTimeout (int64_t v);

    /**
     * 获取socket
     * @param level
     * @param option
     * @param result
     * @param len
     * @return
     */
    bool getOption (int level, int option, void *result, socklen_t *len);

    /**
     * 获取sockopt模板 @see getsockopt
     */
    template<class T>
    bool getOption (int level, int option, T &result) {
        socklen_t length = sizeof (T);
        return getOption(level, option, &result, &length);
    }

    /**
     * 设置sockopt @see getsockopt
     */
     bool setOption (int level, int option, const void *result, socklen_t len);

    template<class T>
    bool setOption (int level, int option, const T &value) {
        return setOption(level, option, &value, sizeof (T));
    }

    /**
     *
     * @return 成功返回新链接的socket，失败返回nullptr
     * Socket必须bind， listen成功
     */
    virtual Socket::ptr accept ();

    /**
     * 绑定地址
     * @param addr 地址
     * @return
     */
    virtual bool bind (const Address::ptr addr);

    /**
     * 连接地址
     * @param addr 目标地址
     * @param timeout_ms 超时时间（毫秒）
     * @return
     */
    virtual bool connect (const Address::ptr addr, uint64_t timeout_ms = -1);
    virtual bool reconnect (uint64_t timeout_ms = -1);

    /**
     * 监听socket
     * backlog 未完成连接的最大长度
     * 返回监听是否成功
     * 必须先bind成功
     */
    virtual bool listen (int backlog = SOMAXCONN);

    /**
     * 关闭socket
     * @return
     */
    virtual bool close ();

    /**
     * 发送数据
     * @param buffer 待发送数据的内存
     * @param length 待发送数据的长度
     * @param flags 标志字
     * @return
     *  > 0 发送成功对应的大小的数据
     *  = 0 socket被关闭
     *  < 0 socket出错
     */
    virtual int send (const void *buffer, size_t length, int flags = 0);

    /**
     * 发送数据
     * @param buffers 待发送数据内存
     * @param length 待发送数据长度
     * @param flags 标志字
     * @return
     *      > 0 发送成功对应大小的数据
     *      = 0 socket被关闭
     *      < 0 socket出错
     */
    virtual int send (const iovec *buffers, size_t length, int flags = 0);

    /**
     * 发送数据
     * @param buffer 待发送数据的内存
     * @param length 待发送数据的长度的
     * @param to 发送的目标地址
     * @param flags 标志字
     * @return
     *      > 0 发送成功对应大小的数据
     *      = 0 socket被关闭
     *      < 0 socket出错
     */
    virtual int sendTo (const void *buffer, size_t length, const Address::ptr to, int flags = 0);

    /**
     * 发送数据
     * @param buffers 待发送数据的内存 （iovec数组）
     * @param length 带发送数据的长度
     * @param to 发送的目标地址
     * @param flags 标志字
     * @return
     *      > 0 发送成功对应大小的数据
     *      = 0 socket被关闭
     *      < 0 socket出错
     */
    virtual int sendTo (const iovec *buffers, size_t length, const Address::ptr to, int flags = 0);

    /**
     * 接受数据
     * @param buffer 接受数据的内存
     * @param length 接受数据内存的大小j
     * @param flags 标志字
     * @return
     *      > 0 接受到对应大小的数据
     *      = 0 socket关闭
     *      < 0 socket出错
     */
    virtual int recv (void *buffer, size_t length, int flags = 0);

    /**
     * 接受数据
     * @param buffers 接受数据的内存
     * @param length 接受数据的内存大小（iovec数组长度）
     * @param flags flags 标志字
     * @return
     *      > 0 接受对应大小的数据
     *      = 0 socket被关闭
     *      < 0 socket出错
     */
    virtual int recv (iovec *buffers, size_t length, int flags = 0);

    /**
     * 接受数据
     * @param buffer 接受数据的内存
     * @param length 接受数据内存的大小
     * @param from 发送端地址
     * @param flags 标志字
     * @return
     *      > 0 接受到数据的大小
     *      = 0 socket被关闭
     *      < 0 socket出错
     */
    virtual int recvFrom (void *buffer, size_t length, Address::ptr from, int flags = 0);

    /**
     * 接受数据
     * @param buffers 接受数据内存（iovec数组)
     * @param length 接受数据的内存大小（iovec数组长度）
     * @param from 发送端地址
     * @param flags 标志字
     * @return
     *      > 0 接受到对应数据的大小
     *      = 0 socket被关闭
     *      < 0 socket出错
     */
    virtual int recvFrom (iovec *buffers, size_t length, Address::ptr from, int flags = 0);

    /**
     * 获取远端地址
     * @return
     */
    Address::ptr getRemoteAddress ();

    /**
     * 获取本地地址
     * @return
     */
    Address::ptr getLocalAddress ();

    /**
     * 获取协议族
     * @return
     */
    int getFamily() const { return m_family; }

    /**
     * 获取类型
     * @return
     */
    int getType () const { return m_type; }

    /**
     * 获取协议
     * @return
     */
    int getProtocol () const { return m_protocol; }

    /**
     * 返回是否链接
     * @return
     */
    bool isConnected () const { return m_isConnected; }

    /**
     * 是否有效（m_sock != 1)
     * @return
     */
    bool isValid() const;

    /**
     * 返回Socket错误
     * @return
     */
    int getError ();

    /**
     * 输出信息到流中
     * @param os
     * @return
     */
    virtual std::ostream &dunmp (std::ostream &os) const ;

    virtual std::string toString () const;

    /**
     * 返回socket句柄
     * @return
     */
    int getSocket () const { return m_sock; }

    /**
     * 取消读
     * @return
     */
    bool cancelRead ();

    /**
     * 取消写
     * @return
     */
    bool cancelWrite();

    /**
     * 取消accept
     * @return
     */
    bool cancelAccept ();

    /**
     * 取消所有事件
     * @return
     */
    bool cancelAll ();

protected:
    /**
     * 初始化socket
     */
    void initSock ();

    /**
     * 创建socket
     */
    void newSock();

    /**
     * 初始化sock
     * @param sock
     * @return
     */
    virtual bool init (int sock);

protected:
    /// socket句柄
    int m_sock;
    /// 协议族
    int m_family;
    /// 类型
    int m_type;
    /// 协议
    int m_protocol;
    /// 是否连接
    int m_isConnected;
    /// 本地地址
    Address::ptr m_localAddress;
    /// 远端地址
    Address::ptr m_remoteAddress;


};
}