/*
    
    date:2023-7-29

    第三方的线程库，Compile and link with -pthread.

    任何线程都能回收其他的已经终止的线程，一般是父线程来回收

    int pthread_join(pthread_t thread, void **retval);
        - 功能：和一个已经终止的线程进行连接，回收子线程的资源，防止产生僵尸线程
                这个函数是阻塞函数，调用一次只能回收一个子线程
                一般在主线程中使用
        - 参数：
            - thread：需要回收的子线程的ID
            - retval: 接收子线程退出时的返回值
        - 返回值：
            0 : 成功
            非0 : 失败，返回的错误号
*/

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

int value = 10; //全局

void *callback(void * arg) {
    printf("child thread id : %ld\n", pthread_self());
    // sleep(2);//阻塞主线程
    
    // int value = 10; // 不要返回局部变量，退出时栈数据消失，返回随机值
    pthread_exit((void *)&value);   // return (void *)&value;
} 

int main() {

    // 创建一个子线程
    pthread_t tid;
    int ret = pthread_create(&tid, NULL, callback, NULL);

    if(ret != 0) {
        char * errstr = strerror(ret);
        printf("error : %s\n", errstr);
    }

    // 主线程
    for(int i = 0; i < 5; i++) {
        printf("%d\n", i);
    }

    printf("tid : %ld, main thread id : %ld\n", tid ,pthread_self());

    // 主线程调用pthread_join()回收子线程的资源

    // pthread_join(tid, NULL); //不需要返回值

    int *thread_retval;
    ret = pthread_join(tid, (void **)&thread_retval);

    if(ret != 0) {
        char * errstr = strerror(ret);
        printf("error : %s\n", errstr);
    }

    printf("reveive child thread exit data : %d\n", *thread_retval);

    printf("回收子线程资源成功！\n");

    // 让主线程退出,当主线程退出时，不会影响其他正常运行的线程。
    pthread_exit(NULL);

    return 0; 
}
