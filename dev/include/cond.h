/*
    date:2023-8-7

    线程同步：条件变量类
*/

#ifndef COND_H
#define COND_H

#include <pthread.h>
#include <exception>

class Cond {
public:
    Cond() {
        if(pthread_cond_init(&m_cond,NULL) != 0) {
            throw std::exception();
        }
    }

    ~Cond() {
        pthread_cond_destroy(&m_cond);
    }

    // 一直等待，调用了该函数，线程会阻塞。阻塞时会自动解锁，解除阻塞时会自动拿锁
    bool wait(pthread_mutex_t *mutex) {
        return pthread_cond_wait(&m_cond, mutex) == 0;
    }

    // 等待多长时间，调用了这个函数，线程会阻塞，直到指定的时间结束。时间到后解除阻塞继续向下运行
    bool timedwait(pthread_mutex_t *mutex, struct timespec t) {
        return pthread_cond_timedwait(&m_cond, mutex, &t) == 0;
    }

    // 唤醒一个或者多个等待的线程
    bool signal() {
        return pthread_cond_signal(&m_cond) == 0;
    }

    // 唤醒所有的等待的线程
    bool broadcast() {
        return pthread_cond_broadcast(&m_cond) == 0;
    }


private:
    pthread_cond_t m_cond;
};



#endif 