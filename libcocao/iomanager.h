#ifndef __LIBCOCAO_IOMANAGER_H__
#define __LIBCOCAO_IOMANAGER_H__

#include "schedule.h"
#include <sys/epoll.h>
#include <cstring>
#include <fcntl.h>
#include "timer.h"

namespace libcocao{

class IOManager: Scheduler, TimerManager {
public:
    typedef std::shared_ptr<IOManager> ptr;
    typedef Mutex MetexType;
    typedef RWMutex RWMutexType;
    enum Event {
        NONE=0x0,
        READ=0x1,
        WRITE=0x2
    };

public:
    struct FdContext {
        typedef Mutex MutexType;

        struct EventContext {
            Scheduler *scheduler = nullptr;
            Fiber::ptr fiber;
            std::function<void()> cb;
        };
        EventContext &getEventContext(Event event);
        void resetEventContext (EventContext &ctx);
        void triggerEvent(Event event);

        EventContext read;
        EventContext write;
        int fd = 0;
        Event events = NONE;
        MutexType mutex;
    };

public:
    IOManager(size_t threads=-1, bool use_caller=true, const std::string &name="");
    ~IOManager() override;
    void contextResize(size_t size);
    void tickle() override;
    void idle() override;
    int addEvent(int fd, Event event, std::function<void()> cb);
    bool delEvent(int fd, Event event);
    bool cancelEvent(int fd, Event event);
    bool cancelAll(int fd);
    bool stopping() override;

private:
    int m_epfd = 0;
    int m_tickleFds[2];
    std::atomic<size_t> m_pendingEventCount = {0};
    RWMutexType m_mutex;
    std::vector<FdContext*> m_fdContexts;
};


}


#endif