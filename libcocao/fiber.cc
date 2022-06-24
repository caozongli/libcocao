#include "fiber.h"
#include "schedule.h"


namespace libcocao {

static Logger::ptr g_logger = LIBCOCAO_LOG_NAME("system");

static thread_local Fiber *t_fiber = nullptr;
static thread_local Fiber::ptr t_thread_fiber = nullptr;

static std::atomic<uint64_t> s_fiber_id {0};
static std::atomic<uint64_t> s_fiber_count{0};

void Fiber::SetThis(Fiber *f) {
    t_fiber = f;
}

Fiber::ptr Fiber::GetThis() {
    if (t_fiber) return t_fiber->shared_from_this();
    Fiber::ptr main_fiber (new Fiber);
    t_thread_fiber = main_fiber;
    return t_fiber->shared_from_this();
}

Fiber::State Fiber::getState() {
    return m_state;
}

Fiber::Fiber() {
    SetThis(this);
    m_state = RUNNING;
    if (getcontext(&m_ctx)) {
        LIBCOCAO_LOG_ERROR(g_logger) << "getcontext error";
    }
    s_fiber_count ++;
    m_id = s_fiber_id ++;
    LIBCOCAO_LOG_DEBUG(g_logger) << "Fiber::Fiber id = " << m_id;
}

Fiber::Fiber(std::function<void()> cb, size_t stacksize, bool run_in_scheduler)
    : m_id (s_fiber_id++)
    , m_cb (cb)
    , m_run_in_scheduler(run_in_scheduler){
    ++ s_fiber_count;
    m_stacksize = stacksize ? stacksize : 1024 * 1024;
    m_stack = malloc(m_stacksize);

    if (getcontext(&m_ctx) ) {
        LIBCOCAO_LOG_ERROR(g_logger) << "getcontext error in Fiber::Fiber";
    }

    m_ctx.uc_link = nullptr;
    m_ctx.uc_stack.ss_sp = m_stack;
    m_ctx.uc_stack.ss_size = m_stacksize;

   makecontext(&m_ctx, &Fiber::MainFunc, 0);
}

Fiber::~Fiber() {
    --s_fiber_count;
    if (m_stack) {
        free(m_stack);
    } else {
        Fiber *cur = t_fiber;
        if (cur == this) {
            SetThis (nullptr);
        }
    }

    LIBCOCAO_LOG_DEBUG(g_logger) << "Fiber::~Fiber id=" << m_id;
}

void Fiber::reset(std::function<void()> cb) {
    m_cb = cb;
    getcontext(&m_ctx);
    m_ctx.uc_link = nullptr;
    m_ctx.uc_stack.ss_sp = m_stack;
    m_ctx.uc_stack.ss_size = m_stacksize;

    makecontext(&m_ctx, &Fiber::MainFunc, 0);
    m_state = READY;
}

void Fiber::yield() {
    SetThis(t_thread_fiber.get());
    if (m_state != TERM) {
        m_state = READY;
    }
    if (m_run_in_scheduler) {
        swapcontext(&m_ctx, &(Scheduler::GetMainFiber()->m_ctx));
    } else {
        swapcontext(&m_ctx, &(t_fiber->m_ctx));
    }
}

void Fiber::resume() {
    SetThis(this);
    if (m_state != TERM) m_state = RUNNING;
    if (m_run_in_scheduler) {
        swapcontext(&(Scheduler::GetMainFiber()->m_ctx), &m_ctx);
    } else {
        swapcontext(&(t_thread_fiber->m_ctx), &m_ctx);
    }
}

void Fiber::MainFunc() {
    Fiber::ptr cur = GetThis();
    cur->m_cb();
    cur->m_cb = nullptr;
    cur->m_state = TERM;

    auto raw_ptr = cur.get();
    cur.reset();
    raw_ptr->yield();
}

uint64_t Fiber::GetFiberId() {
    if (t_fiber) return t_fiber->getId();
    return 0;
}

}
