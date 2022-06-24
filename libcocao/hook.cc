#include "hook.h"

static libcocao::Logger::ptr g_logger = LIBCOCAO_LOG_NAME("system");

namespace libcocao {

static thread_local bool t_hook_enable = false;

#define HOOK_FUN(XX)    \
        XX(sleep)       \
        XX(usleep)      \
        XX(nanosleep)   \
        XX(socket)   \
        XX(connect)   \
        XX(accept)   \
        XX(read)   \
        XX(readv)   \
        XX(recv)   \
        XX(recvfrom)   \
        XX(recvmsg)   \
        XX(write)   \
        XX(writev)   \
        XX(send)   \
        XX(sendto)   \
        XX(sendmsg)   \
        XX(close)   \
        XX(fcntl)   \
        XX(ioctl)   \
        XX(getsockopt)   \
        XX(setsockopt)


void hook_init() {
    static bool is_inited = false;
    if (is_inited) return;
#define XX(name) name ## _f = (name ## _fun)dlsym(RTLD_NEXT, #name);
    HOOK_FUN(XX);
#undef XX
}

static uint64_t s_connect_timeout = -1;

struct _HookIniter {
    _HookIniter() {
        hook_init();
        s_connect_timeout = 5000;
    }
};

static _HookIniter s_hook_initer;

bool is_Hook_enable() {
    return t_hook_enable;
}

void set_hook_enable(bool flag) {
    t_hook_enable = flag;
}

}

struct timer_info {
    int cancelled = 0;
};

template<typename OriginFun, typename... Args>
static ssize_t do_io (int fd, OriginFun fun, const char* hook_fun_name, uint32_t event, int timeout_so, Args &&... args) {
    if (!libcocao::t_hook_enable) {
        return fun(fd, std::forward<Args>(args)...);
    }

    libcocao::FdCtx::ptr ctx = libcocao::FdMgr::GetInstance()->get(fd);

    if (!ctx) {
        int rt = fun(fd, std::forward<Args>(args)...);
        return rt;
    }

    if (ctx->isClose()) {
        errno = EBADF;
        return -1;
    }

    if (!ctx->isSocket() || ctx->getUserNonblock()) {
        return fun(fd, std::forward<Args>(args)...);
    }

    uint64_t to = ctx->getTimeout(timeout_so);      //取出超时时间
    std::shared_ptr<timer_info> tinfo(new timer_info);//设置超时条件

retry:
    ssize_t n = fun(fd, std::forward<Args>(args)...);

    while (n == -1 && errno == EINTR) {
        n = fun(fd, std::forward<Args>(args)...);       //如果读取不成功且是被系统中断，读了多次还没有读到操作会换为EAGIN
    }
    if (n == -1 && errno == EAGAIN) {           //如果读取不成功且需要再次读
        libcocao::IOManager *iom = libcocao::IOManager::GetThis();
        libcocao::Timer::ptr timer;
        std::weak_ptr<timer_info> winfo(tinfo);

        if (to != (uint64_t)-1) {   //超时时间不为-1，即已设置了超时时间
            timer = iom->addConditionTimer(to, [winfo, fd, iom, event]() {  //添加条件超时定时器
                auto t = winfo.lock();  //拿出条件变量并唤醒
                if (!t || t->cancelled) return;     //有这个条件且未被设置错误
                t->cancelled = ETIMEDOUT;  //取消时间不需要做，已经超时了
                iom->cancelEvent(fd, (libcocao::IOManager::Event)event);
            }, winfo);
        }
        int c = 0;
        int rt = iom->addEvent(fd, (libcocao::IOManager::Event)(event));
        if (rt) {
            //添加失败，定时器取消
            if (c) {
                LIBCOCAO_LOG_ERROR(g_logger) << hook_fun_name << " addEvent("
                                                << fd << ", " << event << ")";
            }
            if (timer) {
                timer->cancel();
            }
            return -1;
        } else {
            //添加成功后，让出时间
            LIBCOCAO_LOG_INFO(g_logger) << "do_io" << hook_fun_name << ">";
            libcocao::Fiber::GetThis()->yield();
            LIBCOCAO_LOG_INFO(g_logger) << "do_io" << hook_fun_name << ">";
            if (timer) timer->cancel();
            if (tinfo->cancelled) {
                errno = tinfo->cancelled;
                return -1;
            }
            goto retry;
        }
    }
    return n;
}


extern "C" {
#define XX(name) name ## _fun name ## _f = nullptr;
    HOOK_FUN(XX);
#undef XX

unsigned int sleep(unsigned int seconds) {
    if (!libcocao::t_hook_enable) return sleep_f(seconds);

    libcocao::Fiber::ptr fiber = libcocao::Fiber::GetThis();
    libcocao::IOManager* iom = libcocao::IOManager::GetThis();
    iom->addTimer(seconds * 1000, std::bind((void(libcocao::IOManager::*)
                    (libcocao::Fiber::ptr, int thread))&libcocao::IOManager::schedule, iom, fiber, -1));
    libcocao::Fiber::GetThis()->yield();
    return 0;
}

int usleep(useconds_t usec) {
    if (!libcocao::t_hook_enable) return usleep_f(usec);

    libcocao::Fiber::ptr fiber = libcocao::Fiber::GetThis();
    libcocao::IOManager* iom = libcocao::IOManager::GetThis();
    iom->addTimer(usec / 1000, std::bind((void(libcocao::Scheduler::*)
                    (libcocao::Fiber::ptr, int thread))&libcocao::IOManager::schedule,
                                         iom, fiber, -1));
    libcocao::Fiber::GetThis()->yield();
    return 0;
}

int nanosleep(const struct timespec *req, struct timespec *rem) {
    if (!libcocao::t_hook_enable) return nanosleep_f(req, rem);

    int timeout_ms = req->tv_sec * 1000 + req->tv_nsec / 1000 / 1000;

    libcocao::Fiber::ptr fiber = libcocao::Fiber::GetThis();
    libcocao::IOManager* iom = libcocao::IOManager::GetThis();
    iom->addTimer(timeout_ms, [iom, fiber]() {
        iom->schedule(fiber);
    });
    libcocao::Fiber::GetThis()->yield();
    return 0;
}

int socket(int domain, int type, int protocol) {
    if (!libcocao::t_hook_enable) return socket_f(domain, type, protocol);

    int fd = socket_f(domain, type, protocol);
    if (fd == -1) return fd;

    libcocao::FdMgr::GetInstance()->get(fd, true);
    return fd;
}

int connect_with_timeout(int fd, const struct sockaddr* addr, socklen_t addrlen, uint64_t timeout) {
    if (!libcocao::t_hook_enable) return connect_f(fd, addr, addrlen);
    libcocao::FdCtx::ptr ctx = libcocao::FdMgr::GetInstance()->get(fd);
    if (!ctx || ctx->isClose()) {
        errno = EBADF;
        return -1;
    }

    if (!ctx->isSocket()) return connect_f(fd, addr, addrlen);
    if (ctx->getUserNonblock()) return connect_f(fd, addr, addrlen);

    int n = connect_f(fd, addr, addrlen);
    if (n == 0) return 0;
    else if (n != -1 || errno != EINPROGRESS) return n;

    libcocao::IOManager* iom = libcocao::IOManager::GetThis();
    libcocao::Timer::ptr timer;
    std::shared_ptr<timer_info> tinfo(new timer_info);
    std::weak_ptr<timer_info> winfo(tinfo);

    if (timeout != (uint64_t)-1) {
        timer = iom->addConditionTimer(timeout, [winfo, fd, iom]() {
            auto t = winfo.lock();
            if (!t || t->cancelled) return;
            t->cancelled = ETIMEDOUT;
            iom->cancelEvent(fd, libcocao::IOManager::WRITE);
        }, winfo);
    }

    int rt = iom->addEvent(fd, libcocao::IOManager::WRITE);
    if(rt == 0) {
        libcocao::Fiber::GetThis()->yield();
        if (timer) timer->cancel();
        if (tinfo->cancelled) {
            errno = tinfo->cancelled;
            return -1;
        }
    }  else {
        if (timer) timer->cancel();
        LIBCOCAO_LOG_ERROR(g_logger) << "connect addEvent(" << fd << ", WRITE) error";
    }

    int error = 0;
    socklen_t len = sizeof(int);
    if (-1 == getsockopt(fd, SOL_SOCKET, SO_ERROR, &error, &len)) return -1;

    if(!error) return 0;
    else {
        errno = error;
        return -1;
    }
}

int connect (int sockfd, const struct sockaddr* addr, socklen_t addrlen) {
    return connect_with_timeout(sockfd, addr, addrlen, libcocao::s_connect_timeout);
}

int accept(int s, struct sockaddr* addr, socklen_t *addrlen) {
    int fd = do_io (s, accept_f, "accept", libcocao::IOManager::READ, SO_RCVTIMEO, addr, addrlen);
    if (fd >= 0) libcocao::FdMgr::GetInstance()->get(fd, true);
    return fd;
}

ssize_t read(int fd, void* buf, size_t count) {
    return do_io(fd, read_f, "read", libcocao::IOManager::READ, SO_RCVTIMEO, buf, count);
}

ssize_t readv(int fd, const struct iovec* iov, int iovcnt) {
    return do_io(fd, readv_f, "readv", libcocao::IOManager::READ, SO_RCVTIMEO, iov, iovcnt);
}

ssize_t recv(int sockfd, void* buf, size_t len, int flags) {
    return do_io(sockfd, recv_f, "recv", libcocao::IOManager::READ, SO_RCVTIMEO, buf, len, flags);
}

ssize_t recvfrom(int sockfd, void* buf, size_t len, int flags, struct sockaddr* src_addr, socklen_t *addrlen) {
    return do_io(sockfd, recvfrom_f, "recvfrom", libcocao::IOManager::READ, SO_RCVTIMEO, buf, len, flags, src_addr, addrlen);
}

ssize_t recvmsg(int sockfd, struct msghdr* msg, int flags) {
    return do_io(sockfd, recvmsg_f, "recvmsg", libcocao::IOManager::READ, SO_RCVTIMEO, msg,flags);
}

ssize_t write(int fd, const void *buf, size_t count) {
    return do_io(fd, write_f, "write", libcocao::IOManager::WRITE, SO_SNDTIMEO, buf, count);
}

ssize_t writev(int fd, const struct iovec *iov, int iovcnt) {
    return do_io(fd, writev_f, "writev", libcocao::IOManager::WRITE, SO_SNDTIMEO, iov, iovcnt);
}

ssize_t send(int sockfd, const void *buf, size_t len, int flags) {
    return do_io(sockfd, send_f, "send", libcocao::IOManager::WRITE, SO_SNDTIMEO, buf, len, flags);
}

ssize_t sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen) {
    return do_io(sockfd, sendto_f, "sendto", libcocao::IOManager::WRITE, SO_SNDTIMEO, buf, len, flags, dest_addr, addrlen);
}

ssize_t sendmsg(int sockfd, const struct msghdr *msg, int flags) {
    return do_io(sockfd, sendmsg_f, "sendmsg", libcocao::IOManager::WRITE, SO_SNDTIMEO, msg, flags);
}

int close(int fd) {
    if (libcocao::t_hook_enable) return close_f(fd);

    libcocao::FdCtx::ptr ctx = libcocao::FdMgr::GetInstance()->get(fd);
    if (ctx) {
        auto iom = libcocao::IOManager::GetThis();
        if (iom) iom->cancelAll(fd);
        libcocao::FdMgr::GetInstance()->del(fd);
    }
    return close_f(fd);
}

int fcntl(int fd, int cmd, ... /* arg */ ) {
//    if (!libcocao::t_hook_enable) return fcntl_f(fd, cmd);

    va_list va;
    va_start(va, cmd);
    switch (cmd) {
        case F_SETFL:
        {
            int arg = va_arg(va, int);
            va_end(va);
            libcocao::FdCtx::ptr ctx = libcocao::FdMgr::GetInstance()->get(fd);
            if (!ctx || ctx->isClose() || !ctx->isSocket()) {
                return fcntl_f(fd, cmd, arg);
            }
            ctx->setUserNonblock(arg & O_NONBLOCK);
            if (ctx->getSysNonblock()) {
                arg |= O_NONBLOCK;
            } else {
                arg &= ~O_NONBLOCK;
            }
            return fcntl_f(fd, cmd, arg);
        }
            break;
        case F_GETFL:
        {
            va_end(va);
            int arg = fcntl_f(fd, cmd);
            libcocao::FdCtx::ptr ctx = libcocao::FdMgr ::GetInstance()->get(fd);
            if (!ctx || ctx->isClose() || !ctx->isSocket()) return arg;
            if (ctx->getUserNonblock()) return arg | O_NONBLOCK;
            else return arg & ~O_NONBLOCK;
        }
            break;

        case F_DUPFD:
        case F_DUPFD_CLOEXEC:
        case F_SETFD:
        case F_SETSIG:
        case F_SETLEASE:
        case F_NOTIFY:
        case F_SETPIPE_SZ:
        {
            int arg = va_arg(va, int);
            va_end(va);
            return fcntl_f(fd, cmd, arg);
        }
            break;

        case F_GETFD:
        case F_GETOWN:
        case F_GETSIG:
        case F_GETLEASE:
        case F_GETPIPE_SZ:
        {
            va_end(va) ;
            return fcntl_f(fd, cmd);
        }
            break;

        case F_SETLK:
        case F_SETLKW:
        case F_GETLK:
        {
            struct flock* arg = va_arg(va, struct flock*);
            va_end(va);
            return fcntl_f(fd, cmd, arg);
        }
            break;

        case F_GETOWN_EX:
        case F_SETOWN_EX:
        {
            struct f_owner_exlock* arg = va_arg(va, struct f_owner_exlock*);
            va_end(va);
            return fcntl_f(fd, cmd, arg);
        }
            break;
        default:
            va_end(va);
            return fcntl_f(fd, cmd);
    }
}

int ioctl(int fd, unsigned long request, ...) {
    va_list va;
    va_start(va, request);
    void *arg = va_arg(va, void*);
    va_end(va);

    if (FIONBIO == request) {
        bool user_nonblock = !!*(int*)arg;
        libcocao::FdCtx::ptr ctx = libcocao::FdMgr::GetInstance()->get(fd);
        if (!ctx || ctx->isClose() || !ctx->isSocket()) return ioctl_f(fd, request, arg);
        ctx->setUserNonblock(user_nonblock);
    }
    return ioctl_f(fd, request, arg);
}

int getsockopt(int sockfd, int level, int optname, void *optval, socklen_t *optlen){
    return getsockopt_f(sockfd, level, optname, optval, optlen);
}

int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen) {
    if (!libcocao::t_hook_enable) return setsockopt_f(sockfd, level, optname, optval, optlen);
    if (level == SOL_SOCKET) {}
    if (optname == SO_RCVTIMEO || SO_SNDTIMEO) {
        libcocao::FdCtx::ptr ctx = libcocao::FdMgr::GetInstance()->get(sockfd);
        if (ctx) {
            const timeval * v = (const timeval*)optval;
            ctx->setTimeoout(optname, v->tv_sec * 1000 + v->tv_usec / 1000);
        }
    }
    return setsockopt_f(sockfd, level, optname, optval, optlen);
}



}
