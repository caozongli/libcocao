#include "thread.h"
#include "log.h"

namespace libcocao {

static thread_local Thread* t_thread = nullptr;
static thread_local std::string t_thread_name = "UNKONW";

libcocao::Logger::ptr g_logger = LIBCOCAO_LOG_NAME("system");



Thread::Thread(std::function<void()> cb, const std::string& name)
    : m_cb(cb)
    , m_name(name) {
    if (name.empty()) {
        m_name = "UNKONW";
    }

    int rt = pthread_create(&m_thread, nullptr, &Thread::run, this);
    if (rt) {
        LIBCOCAO_LOG_ERROR(g_logger) << "pthread_create thread fail, rt = " << rt << "name=" << name;
        throw std::logic_error("pthread_create error");
    }
    m_semaphore.wait();
}

Thread::~Thread() {
    if (m_thread) {
        pthread_detach(m_thread);
    }
}

pid_t Thread::getId() const {
    return m_id;
}

const std::string& Thread::getName() const {
    return m_name;
}

void Thread::join() {
    if (m_thread) pthread_join(m_thread, nullptr);
}

void* Thread::run(void *arg) {
    Thread* thread = (Thread*)arg;
    t_thread = thread;
    t_thread_name = thread->m_name;
    thread->m_id = libcocao::GetThreadId();

    pthread_setname_np(pthread_self(), thread->m_name.substr(0, 15).c_str());
    std::function<void()> cb;
    cb.swap(thread->m_cb);
    thread->m_semaphore.notity();
    cb();
    return 0;
}

Thread* Thread::GetThis() {
    return t_thread;
}

const std::string& Thread::GetName() {
    return t_thread_name;
}

void Thread::setName(const std::string &name) {
    if (t_thread) {
        t_thread->m_name = name;
    }
    t_thread_name = name;
}

}