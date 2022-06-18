#ifndef __LIBCOCAO_FD_MANAGER_H__
#define __LIBCOCAO_FD_MANAGER_H__
#include <memory>
#include "thread.h"
#include "iomanager.h"
#include "singleton.h"

namespace libcocao {
class FdCtx : public std::enable_shared_from_this<FdCtx> {
public:
    typedef std::shared_ptr<FdCtx> ptr;
    FdCtx(int fd);
    ~FdCtx();

    bool init();
    bool isInit() const { return m_isInit; }
    bool isSocket() const { return m_isSocket; }
    bool isClose() const { return m_isClosed; }

    void setUserNonblock(bool v) { m_userNonblock = v; }
    bool getUserNonblock() const { return m_userNonblock; }

    void setSysNonblock(bool v) { m_sysNonblock = v;}
    bool getSysNonblock() const { return m_sysNonblock; }

    void setTimeoout( int type, uint64_t v);
    uint64_t getTimeout(int type);


private:
    //是否初始化
    bool m_isInit: 1;
    //是否为socket
    bool m_isSocket: 1;
    //是否hook非阻塞
    bool m_sysNonblock: 1;
    //是否用户主动设置非阻塞
    bool m_userNonblock: 1;
    //是否关闭
    bool m_isClosed: 1;
    //文件句柄
    int m_fd;
    //读超时毫秒
    uint64_t m_recvTimeout;
    //写超时毫秒
    uint64_t m_sendTimeout;
    libcocao::IOManager* m_iomanager;
};

}


#endif