/*

    date:2023-7-30

    当有一个线程已经持有互斥锁时，互斥锁将所有试图进入临界区的线程都阻塞住。但是考
    虑一种情形，当前持有互斥锁的线程只是要读访问共享资源，而同时有其它几个线程也想
    读取这个共享资源，但是由于互斥锁的排它性，所有其它线程都无法获取锁，也就无法读
    访问共享资源了，但是实际上多个线程同时读访问共享资源并不会导致问题。
        
        ◼ 在对数据的读写操作中，更多的是读操作，写操作较少，例如对数据库数据的读写应用。
            为了满足当前能够允许多个读出，但只允许一个写入的需求，线程提供了读写锁来实现。
        ◼ 读写锁的特点：
            如果有其它线程读数据，则允许其它线程执行读操作，但不允许写操作。
            如果有其它线程写数据，则其它线程都不允许读、写操作。
            写是独占的，写的优先级高。

    读写锁的类型 pthread_rwlock_t
    int pthread_rwlock_init(pthread_rwlock_t *restrict rwlock, const pthread_rwlockattr_t *restrict attr);
    int pthread_rwlock_destroy(pthread_rwlock_t *rwlock);
    int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock);
    int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock);
    int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock);
    int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock);
    int pthread_rwlock_unlock(pthread_rwlock_t *rwlock);
    

    案例：8个线程操作同一个全局变量。
    3个线程不定时写这个全局变量，5个线程不定时的读这个全局变量
*/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// 创建一个共享数据
int num = 1;

// pthread_mutex_t mutex;
pthread_rwlock_t rwlock;

void *writeNum(void * arg) {

    while(1) {
        pthread_rwlock_wrlock(&rwlock);
        num++;
        printf("++write, tid : %ld, num : %d\n", pthread_self(), num);
        pthread_rwlock_unlock(&rwlock);
        usleep(100);
    }

    return NULL;
}

void *readNum(void * arg) {

    while(1) {
        pthread_rwlock_rdlock(&rwlock);
        printf("==read, tid : %ld, num : %d\n", pthread_self(), num);
        pthread_rwlock_unlock(&rwlock);
        usleep(100);
    }

    return NULL;
}

int main() {
    //初始化读写锁
    pthread_rwlock_init(&rwlock, NULL);

    // 创建3个写线程，5个读线程
    pthread_t wtids[3], rtids[5];

    for(int i = 0; i < 3; i++) {
        pthread_create(&wtids[i], NULL, writeNum, NULL);
    }

    for(int i = 0; i < 5; i++) {
        pthread_create(&rtids[i], NULL, readNum, NULL);
    }

    // 设置线程分离
    for(int i = 0; i < 3; i++) {
       pthread_detach(wtids[i]);
    }

    for(int i = 0; i < 5; i++) {
         pthread_detach(rtids[i]);
    }

    pthread_exit(NULL);

    pthread_rwlock_destroy(&rwlock);

    return 0;
}