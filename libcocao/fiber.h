#ifndef __LBICOCAO_FIBER_H__
#define __LIBCOCAO_FIBER_H__

#include <ucontext.h>
#include <functional>
#include <memory>

namespace libcocao {

class Fiber : public std::enable_shared_from_this<Fiber>{
public:
    typedef std::shared_ptr<Fiber> ptr;
    enum State {
        RUNNING,
        TERM,
        READY
    };

private:
    Fiber();

public:
    Fiber(std::function<void()> cb, size_t stacksize = 0, bool run_in_scheduler = false);
    ~Fiber();

    void reset (std::function<void()> cb);
    void yield();
    void resume();
    State getState();

public:
    static void SetThis(Fiber* f);
    static Fiber::ptr GetThis();
    static void MainFunc();

private:
    uint64_t m_id = 0;
    uint32_t m_stacksize = 0;
    State m_state = READY;

    ucontext_t m_ctx;
    void* m_stack = nullptr;

    std::function<void()> m_cb;
    bool m_run_in_scheduler;
};

}

#endif