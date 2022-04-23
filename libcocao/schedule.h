#ifndef __LIBCOCAO_SCHEDULE_H__
#define __LIBCOCAO_SCHEDULE_H__

#include <atomic>
#include <functional>
#include <memory>
#include <vector>
#include "fiber.h"
#include "thread.h"
#include "log.h"

namespace libcocao {

class Scheduler {
public:
    typedef std::shared_ptr<Scheduler> ptr;
    typedef Mutex MutexType;

    Scheduler (size_t threads, bool use_caller, const std::string &name);
    void start();
    bool stopping();
    void tickle();
    void idle();
    void run();
    void stop();
    void SetThis();
    bool hasIdleThreads() {return m_idleThreadCount > 0;}
    const std::string &getName() { return m_name; }

    template<class FiberOrCb>
    void schedule (FiberOrCb fc, int thread = -1) {
        bool need_tickle = false;
        {
            MutexType::Lock lock (m_mutex);
            need_tickle = scheduleNoLock(fc, thread);
        }
        if (need_tickle) {
            tickle();
        }
    }

    template<class InputItertor>
    void schedule (InputItertor begin, InputItertor end) {
        bool need_tickle = false;
        {
            MutexType::Lock lock(m_mutex);
            while (begin != end) {
                need_tickle = scheduleNoLock(&*begin++, -1) || need_tickle;
            }
            if (need_tickle) {
                tickle();
            }
        }
    }


    static Scheduler* GetThis();
    static Fiber* GetMainFiber();

private:
    struct SchedulerTask {
        Fiber::ptr fiber;
        std::function<void()> cb;
        int thread;

        SchedulerTask(Fiber::ptr *f, int thr) {
            fiber = *f;
            thread = thr;
        };
        SchedulerTask(Fiber::ptr f, int thr) {
            fiber.swap(f);
            thread = thr;
        }
        SchedulerTask(std::function<void()> *f, int thr) {
            cb = f;
            thread = thr;
        }
        SchedulerTask ()  {thread = -1;}
        void reset () {
            fiber = nullptr;
            cb = nullptr;
            thread = -1;
        }

    };

private:
    template<class FiberOrCb>
    bool scheduleNoLock (FiberOrCb fc, int thread) {
        bool need_tickle = m_tasks.empty();
        SchedulerTask ft(fc, thread);
        if (ft.fiber ||ft.cb) {
            m_tasks.push_back(ft);
        }
        return need_tickle;
    }

private:
    std::string m_name;
    MutexType m_mutex;
    std::vector<Thread::ptr> m_threads;  //线程池
    std::list<SchedulerTask> m_tasks; //任务队列
    std::vector<int> m_threadIds;   //线程id数组
    Fiber::ptr m_rootFiber;
    size_t m_threadCount = 0;       //线程总数,不包含user_caller主线程
    std::atomic<size_t> m_activateThreadCount{0};   //活跃线程数量
    std::atomic<size_t> m_idleThreadCount{0};       //空闲线程数量
    bool m_useCaller;                               //是否为usercaller
    int m_rootThread = 0;                           //usercaller为true时，调度器所在线程id
    bool m_stopping = false;                         //是否正在停止
};

}

#endif