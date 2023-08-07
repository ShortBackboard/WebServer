/*
    date:2023-8-7

    线程同步：互斥锁类
*/

#ifndef LOCKER_H
#define LOCKER_H

#include <pthread.h>
#include <exception>


class Locker {
public:
    // 构造方法
    Locker() {
        // 初始化互斥量
        if(pthread_mutex_init(&m_mutex, NULL) != 0) {
            // 创建失败抛出异常
            throw std::exception();
        }
    }

    // 析构
    ~Locker() {
        pthread_mutex_destroy(&m_mutex);
    }

    // 拿锁
    bool lock() {
        // pthread_mutex_lock返回0说明拿到锁
        return pthread_mutex_lock(&m_mutex) == 0;
    }

    // 解锁
    bool unlock() {
        return pthread_mutex_unlock(&m_mutex) == 0;
    }

    // 获得互斥量成员
    pthread_mutex_t *get() {
        return &m_mutex;
    }

private:
    pthread_mutex_t m_mutex;
};




#endif 