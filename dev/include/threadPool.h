/*
    date:2023-8-7

    线程池类
    
    模板定义声明最好在一个文件，不然可能会报错
*/

#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <list>
#include "locker.h"
#include "cond.h"
#include "sem.h"
#include "httpConn.h"


// 定义成模板类，方便后面代码的复用
// 模板参数T在本项目中为任务类

template<typename T>
class ThreadPool {
public:
    ThreadPool(int thread_num = 8, int max_requests = 10000);

    ~ThreadPool();

    // 向请求队列添加任务
    bool append(T* request);


private:
    // 静态函数，创建线程时使用
    static void *worker(void *arg);

    // 子线程创建后的工作函数
    void run();



private:
    // 线程池初始化的线程数量
    int m_thread_number;

    // 线程池数组容器，大小为线程池初始化的线程数量
    pthread_t *m_threads;

    // 请求队列中最大允许的等待处理的线程数量
    int m_max_requests;

    // 请求队列
    std::list< T*> m_workQueue;

    // 互斥锁
    Locker m_queueLocker;

    // 信号量,判断是否有任务需要处理
    Sem m_queuestat;

    // 是否结束所有线程
    bool m_stop;

};

// 构造函数
template<typename T>
ThreadPool<T>::ThreadPool(int thread_num, int max_requests):
    m_thread_number(thread_num), m_max_requests(max_requests),
    m_stop(false), m_threads(NULL)
{
    if(thread_num <= 0 || max_requests <= 0) {
        throw std::exception();
    }

    // 初始化线程池容器
    m_threads = new pthread_t[m_thread_number];

    // 如果创建失败
    if(!m_threads) {
        throw std::exception();
    }

    // 创建m_thread_number个线程，并将他们设置成线程分离
    for (int i = 0; i < m_thread_number; i++)
    {
        printf("create the %dth thread\n", i);

        // 作为this指针参数传入worker
        if(pthread_create(m_threads + i, NULL, worker, this) != 0) {
            // 创建失败
            delete []m_threads;
            throw std::exception();
        }

        // 将他们设置成线程分离
        if(pthread_detach(m_threads[i]) != 0) {
            // 分离失败
            delete []m_threads;
            throw std::exception();
        }
    }
}


// 向请求队列添加任务
template<typename T>
bool ThreadPool<T>::append(T* request) {

    m_queueLocker.lock();

    // 如果超出最大允许的等待处理的线程数量
    if(m_workQueue.size() > m_max_requests) {
        m_queueLocker.unlock();
        return false;
    }

    m_workQueue.push_back(request);
    m_queueLocker.unlock();
    m_queuestat.post();
    
    return true;
}


// 静态函数，创建线程时使用
template<typename T>
void *ThreadPool<T>::worker(void *arg) {
    ThreadPool *pool = (ThreadPool *) arg;
    
    pool->run();

    return pool;
}


// 子线程创建后的工作函数
template<typename T>
void ThreadPool<T>::run() {
    while(!m_stop) {
        // 对信号量加锁，调用一次对信号量的值-1，如果值为0，就阻塞
        m_queuestat.wait();

        m_queueLocker.lock();
        
        // 请求队列为空，此时还没有客户端连接，各个子线程一直运行到此循环
        if(m_workQueue.empty()) {
            m_queueLocker.unlock();
            continue;
        }

        // 取请求队列第一个数据
        T* request = m_workQueue.front();

        // 取请求队列数据后删除第一个数据
        m_workQueue.pop_front();

        m_queueLocker.unlock();

        // 如果没有取到数据
        if(!request) {
            continue;
        }

        // 如果该子线程取到了数据，对请求报文进行处理
        request->process();

    }
}


// 析构函数
template<typename T>
ThreadPool<T>::~ThreadPool() {
    delete []m_threads;
    m_stop = true;
}


#endif 