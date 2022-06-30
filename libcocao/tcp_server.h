#ifndef __LIBCOCAO_TCP_SERVER_H__
#define __LIBCOCAO_TCP_SERVER_H__

#include <memory>
#include <functional>
#include "iomanager.h"
#include "socket.h"
#include "noncopyable.h"

namespace libcocao {

class TcpServer : public std::enable_shared_from_this<TcpServer>
                , Noncopyable {
public:
    typedef std::shared_ptr<TcpServer> ptr;

    /**
     * 构造函数
     * @param name 服务器名称
     * #param type 服务器类型
     * @param io_worket socket客户端工作的协程调度器
     * @param accept_worker 服务器socket执行接收socket连接的协程调度器
     */
    TcpServer (libcocao::IOManager* io_worket = libcocao::IOManager::GetThis()
                , libcocao::IOManager *accept_worker = libcocao::IOManager::GetThis());

    /**
     * 析构函数
     */
    virtual ~TcpServer();

    /**
     * 绑定地址
     * @param addr
     * @return 返回是否绑定成功
     */
    virtual bool bind (libcocao::Address::ptr addr);

    /**
     * 绑定地址数组
     * @param addr 需要绑定的地址数组
     * @param fails 绑定失败的地址
     * @return 是否绑定成功
     */
    virtual bool bind (const std::vector<Address::ptr>& addr
                        , std::vector<Address::ptr>& fails);

    /**
     * 启动服务
     * @return 需要bind成功后执行
     */
    virtual bool start ();

    /**
     * 停止服务
     */
    virtual void stop();

    /**
     * 返回读取超时时间（毫秒）
     */
     uint64_t getRecvTimeout () const { return m_recvTimeout; }

     /**
      * 返回服务器名称
      * @return
      */
     std::string getName () const { return m_name; }

     /**
      * 设置读取超时时间（毫秒）
      * @param v
      */
     void setRecvTimeout (uint64_t v) { m_recvTimeout = v;}

     /**
      * 设置服务器名称
      * @param v
      */
     virtual void setName (const std::string &v) { m_name = v;}

     /**
      * 以字符串形式dump server信息
      * @param prefix
      * @return
      */
     virtual std::string toString (const std::string &prefix = "");

protected:
    /**
     * 处理新连接的Socket类
     * @param client
     */
    virtual void handleClient (Socket::ptr client);

    /**
     * 开始接受连接
     * @param sock
     */
    virtual void startAccept (Socket::ptr sock);

protected:
    /// 监听Socket数组
    std::vector<Socket::ptr> m_socks;
    /// 新连接的Socket工作的调度器
    IOManager *m_ioworker;
    /// 服务器Socket接收连接的调度器
    IOManager *m_acceptWorker;
    /// 接受超时时间（毫秒）
    uint64_t m_recvTimeout;
    /// 服务器名称
    std::string m_name;
    /// 服务器类型
    std::string m_type;
    /// 服务是否停止
    bool m_isStop;

};

}

#endif