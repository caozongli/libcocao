#ifndef __LIBCOCAO_THREAD_H__
#define __LIBCOCAO_THREAD_H__

#include "mutex.h"

namespace libcocao {

class Thread: public Noncopyable{
public:
    typedef std::shared_ptr<Thread> ptr;
    Thread(std::function<void()> cb, const std::string& name);
    ~Thread();

    pid_t getId() const;
    const std::string& getName() const;

    void join();

    static Thread* GetThis();
    static const std::string& GetName();
    static void setName(const std::string& name);

private:
    static void* run(void* arg);

private:
    pid_t m_id;
    pthread_t m_thread = 0;
    std::function<void()> m_cb;
    std::string m_name;

    Semaphore m_semaphore;
};

}

#endif
