#ifndef __LIBCOCAO_SCHEDULE_H__
#define __LIBCOCAO_SCHEDULE_H__

#include <atomic>
#include <functional>
#include <memory>
#include <vector>
#include "fiber.h"
#include "thread.h"

namespace libcocao {

class Scheduler {
public:
    typedef std::shared_ptr<Scheduler> ptr;
    typedef Mutex MutexType;

    Scheduler (size_t threads, bool use_caller, const std::string &name);
    void start();

    virtual bool stopping();

    virtual void tickle();

    virtual void idle();
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
    struct ScheduleTask {
        Fiber::ptr fiber;
        std::function<void()> cb;
        int thread;

        ScheduleTask(Fiber::ptr f, int thr) {
            fiber  = f;
            thread = thr;
        }
        ScheduleTask(Fiber::ptr *f, int thr) {
            fiber.swap(*f);
            thread = thr;
        }
        ScheduleTask(std::function<void()> f, int thr) {
            cb     = f;
            thread = thr;
        }
        ScheduleTask(std::function<void()>* f, int thr) {
            cb.swap(*f);
            thread = thr;
        }
        ScheduleTask() { thread = -1; }

        void reset() {
            fiber  = nullptr;
            cb     = nullptr;
            thread = -1;
        }
    };

private:
    template<class FiberOrCb>
    bool scheduleNoLock (FiberOrCb fc, int thread) {
        bool need_tickle = m_tasks.empty();
        ScheduleTask task(fc, thread);
        if (task.fiber || task.cb) {
            m_tasks.push_back(task);
        }
        return need_tickle;
    }

private:
    std::string m_name;
    MutexType m_mutex;
    std::vector<Thread::ptr> m_threads;  //?????????
    std::list<ScheduleTask> m_tasks; //????????????
    std::vector<int> m_threadIds;   //??????id??????
    Fiber::ptr m_rootFiber;
    size_t m_threadCount = 0;       //????????????,?????????user_caller?????????
    std::atomic<size_t> m_activateThreadCount{0};   //??????????????????
    std::atomic<size_t> m_idleThreadCount{0};       //??????????????????
    bool m_useCaller;                               //?????????usercaller
    int m_rootThread = 0;                           //usercaller???true???????????????????????????id
    bool m_stopping = false;                         //??????????????????
};

}

#endif