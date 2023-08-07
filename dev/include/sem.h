/*
    date:2023-8-7

    线程同步：信号量类
*/

#ifndef SEM_H
#define SEM_H

#include <semaphore.h>
#include <exception>

class Sem {
public:
    Sem() {
        if(sem_init(&m_sem, 0, 0) != 0) {
            throw std::exception();
        }
    }

    Sem(int num) {
        if(sem_init(&m_sem, 0, num) != 0) {
            throw std::exception();
        }
    }

    ~Sem() {
        sem_destroy(&m_sem);
    }

    // 对信号量加锁，调用一次对信号量的值-1，如果值为0，就阻塞
    bool wait() {
        return sem_wait(&m_sem) == 0;
    }

    // 对信号量解锁，调用一次对信号量的值+1
    bool post() {
        return sem_post(&m_sem) == 0;
    }

private:
    sem_t m_sem;
};




#endif 