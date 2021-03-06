#include "schedule.h"
#include "hook.h"

namespace libcocao {

static Logger::ptr g_logger = LIBCOCAO_LOG_NAME("system");
//当前线程的调度器，同一个调度器下的所有线程共享同一个实例
static thread_local Scheduler *t_scheduler = nullptr;
//当前线程的调度协程，每个线程都独有一份
static thread_local Fiber *t_scheduler_fiber = nullptr;

Scheduler::Scheduler(size_t threads, bool use_caller, const std::string &name) {
    m_useCaller = use_caller;
    m_name = name;

    if (m_useCaller) {
        -- threads;
        libcocao::Fiber::GetThis();
        t_scheduler = this;
        m_rootFiber.reset(new Fiber(std::bind(&Scheduler::run, this), 0, false));
        libcocao::Thread::setName (m_name);
        t_scheduler_fiber = m_rootFiber.get();
        m_rootThread = libcocao::GetThreadId();
        m_threadIds.push_back(m_rootThread);
    } else {
        m_rootThread = -1;
    }
    m_threadCount = threads;
}

Scheduler *Scheduler::GetThis() {
    return t_scheduler;
}

void Scheduler::SetThis() {
    t_scheduler = this;
}

Fiber *Scheduler::GetMainFiber() {
    return t_scheduler_fiber;
}

void Scheduler::start() {
    LIBCOCAO_LOG_DEBUG(g_logger) << "start";
    MutexType::Lock lock(m_mutex);
    if (m_stopping) {
        LIBCOCAO_LOG_ERROR(g_logger) << "Scheduler is stopped";
        return;
    }
    m_threads.resize(m_threadCount);
    for (size_t i = 0; i < m_threadCount; ++ i) {
        m_threads[i].reset(new Thread(std::bind(&Scheduler::run, this), m_name + "_" + std::to_string(i)));
        m_threadIds.push_back(m_threads[i]->getId());
    }
}

bool Scheduler::stopping() {
    MutexType::Lock lock(m_mutex);
    return m_stopping && m_tasks.empty() && m_activateThreadCount == 0;
}

void Scheduler::tickle() {
    LIBCOCAO_LOG_DEBUG(g_logger) << "tickle";
}

void Scheduler::idle() {
    while (!stopping()) libcocao::Fiber::GetThis()->yield();
}

void Scheduler::run() {

    set_hook_enable(true);
    SetThis();

    if (libcocao::GetThreadId() != m_rootThread) {
        t_scheduler_fiber = libcocao::Fiber::GetThis().get();
    }

    Fiber::ptr idle_fiber(new Fiber(std::bind(&Scheduler::idle, this)));
    Fiber::ptr cb_fiber;

    ScheduleTask task;

    while (true) {
        task.reset();
        bool tickle_me = false;
        {
            MutexType::Lock lock(m_mutex);
            auto it = m_tasks.begin();
            while (it != m_tasks.end()) {
                if (it->thread != -1 && it->thread != libcocao::GetThreadId()) {
                    ++ it;
                    tickle_me = true;
                    continue;
                }
//                if (it->fiber) {
//                }

                if(it->fiber && it->fiber->getState() == Fiber::RUNNING) {
                    ++it;
                    continue;
                }

                task = *it;
                m_tasks.erase(it ++);
                ++ m_activateThreadCount;
                break;
            }
            tickle_me |= (it != m_tasks.end());
        }
        if (tickle_me) {
            tickle();
        }

        if (task.fiber) {
            task.fiber->resume();
            -- m_activateThreadCount;
            task.reset();
        } else if (task.cb) {
            if (cb_fiber) {
                cb_fiber->reset(task.cb);
            } else {
                cb_fiber.reset(new Fiber(task.cb));
            }
            task.reset();
            cb_fiber->resume();
            -- m_activateThreadCount;
            cb_fiber.reset();
        } else {
            if (idle_fiber->getState() == Fiber::TERM) {
                LIBCOCAO_LOG_DEBUG(g_logger) << "idle fiber term";
                break;
            }
            ++ m_idleThreadCount;
            idle_fiber->resume();
            -- m_idleThreadCount;
        }
    }
    LIBCOCAO_LOG_DEBUG(g_logger) << "Scheduler::run() exit";
}

void Scheduler::stop() {
    if (stopping())
        return;
    m_stopping = true;

    for (size_t i = 0; i < m_threadCount; ++ i) tickle();

    if (m_rootFiber) tickle();

    if (m_rootFiber) {
        m_rootFiber->resume();
    }

    std::vector<Thread::ptr> thrs;
    {
        MutexType::Lock lock(m_mutex);
        thrs.swap(m_threads);
    }
    for (auto &i :thrs) i->join();
}

}