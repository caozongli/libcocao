#ifndef __LIBCOCAO_MUTEX_H__
#define __LIBCOCAO_MUTEX_H__

#include <thread>
#include <functional>
#include <memory>
#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <atomic>
#include <list>

#include "noncopyable.h"

namespace libcocao {
class Semaphore: public Noncopyable {
public:
    Semaphore(uint32_t count = 0);
    ~Semaphore();

    void wait();
    void notity();

private:
    sem_t m_semaphore;
};

template <class T>
class ScopeLockImp1 {
public:
    ScopeLockImp1(T& mutex)
            : m_mutex(mutex) {
        m_mutex.lock();
        m_locked = true;
    }
    ~ScopeLockImp1() {
        unlock();
    }
    void lock() {
        if (!m_locked) {
            m_mutex.lock();
            m_locked = true;
        }
    }
    void unlock() {
        if (m_locked) {
            m_mutex.unlock();
            m_locked = false;
        }
    }
private:
    T& m_mutex;
    bool m_locked;
};

class Mutex: Noncopyable {
public:
    typedef ScopeLockImp1<Mutex> Lock;
    Mutex() {
        pthread_mutex_init(&m_mutex, nullptr);
    }
    ~Mutex() {
        pthread_mutex_destroy(&m_mutex);
    }
    void lock() {
        pthread_mutex_lock(&m_mutex);
    }
    void unlock() {
        pthread_mutex_unlock(&m_mutex);
    }
private:
    pthread_mutex_t m_mutex;
};

template<class T>
class ReadScopeLockImp1 {
public:
    ReadScopeLockImp1(T& mutex)
            : m_mutex(mutex){
        m_mutex.rdlock();
        m_locked = true;
    }
    ~ReadScopeLockImp1() {
        unlock();
    }
    void lock() {
        if (!m_locked) {
            m_mutex.rdlock();
            m_locked = true;
        }
    }
    void unlock() {
        if (m_locked) {
            m_mutex.wrlock();
            m_locked = false;
        }
    }
private:
    T& m_mutex;
    bool m_locked;
};

template<class T>
class WriteScopeLockImp1 {
public:
    WriteScopeLockImp1(T& mutex)
            : m_mutex(mutex){
        m_mutex.wrlock();
        m_locked = true;
    }
    ~WriteScopeLockImp1() {
        unlock();
    }
    void lock() {
        if (!m_locked) {
            m_mutex.wrlock();
            m_locked = true;
        }
    }
    void unlock() {
        if (m_locked) {
            m_mutex.wrlock();
            m_locked = false;
        }
    }
private:
    T& m_mutex;
    bool m_locked;
};

class RWMutex: Noncopyable {
public:
    typedef ReadScopeLockImp1<RWMutex> ReadLock;
    typedef WriteScopeLockImp1<RWMutex> WriteLock;
    RWMutex() {
        pthread_rwlock_init(&m_lock, nullptr);
    }
    ~RWMutex() {
        pthread_rwlock_destroy(&m_lock);
    }
    void rdlock() {
        pthread_rwlock_rdlock(&m_lock);
    }
    void wrlock() {
        pthread_rwlock_wrlock(&m_lock);
    }
    void unlock() {
        pthread_rwlock_unlock(&m_lock);
    }
private:
    pthread_rwlock_t m_lock;
};

class SpinLock: Noncopyable {
public:
    typedef ScopeLockImp1<SpinLock> Lock;
    SpinLock() {
        pthread_spin_init(&m_mutex, 0);
    }
    ~SpinLock() {
        pthread_spin_destroy(&m_mutex);
    }
    void lock() {
        pthread_spin_lock(&m_mutex);
    }
    void unlock() {
        pthread_spin_unlock(&m_mutex);
    }
private:
    pthread_spinlock_t m_mutex;
};

class CASLock:Noncopyable {
public:
    typedef ScopeLockImp1<CASLock> Lock;
    CASLock() {
        m_mutex.clear();
    }
    ~CASLock(){}
    void lock() {
        while (m_mutex.test_and_set(std::memory_order_acquire));
    }
    void unlock() {
        m_mutex.clear(std::memory_order_release);
    }

private:
    volatile std::atomic_flag m_mutex;
};
}
#endif