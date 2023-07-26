/*
    date:2023-7-26

    int setitimer(int which, const struct itimerval *new_value,
                     struct itimerval *old_value);

    功能:设置定时器。
    可以替代alarm函数，精度微秒us，可以实现周期性定时

    参数：
        -which：定时器以什么时间计时
            ITIMER_REAL    ：真实时间，发送SIGALRM信号，常用
            ITIMER_VIRTUAL ：用户时间，发送SIGVTALRM信号
            ITIMER_PROF ：用户态和内核态时间之和，发送SIGPROF信号

        -*new_value：设置定时器属性

        struct itimerval { //定时器的结构体
               struct timeval it_interval; //间隔时间
               struct timeval it_value;    //延迟多少时间
           };

        struct timeval { //时间的结构体
               time_t      tv_sec;       //秒数
               suseconds_t tv_usec;     //微秒   
           };

        eg：过10s后每隔2s定时一次

        -*old_value：上一次定时器的信息，一般指定为NULL，不使用
                传出参数


    返回值：
        On success, zero is returned. 
        On error, -1 is returned, and errno is set appropriately.


    
*/

#include <sys/time.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>


// 过3秒以后，每隔2秒定时一次
int main() {
    struct itimerval new_value;

    //设置间隔时间
    new_value.it_interval.tv_sec = 2;
    new_value.it_interval.tv_usec = 0;

    //设置延迟时间
    new_value.it_value.tv_sec = 3;
    new_value.it_value.tv_usec = 0;
    
    //到时会发送SIGALRM信号,默认处理是结束进程
    int ret = setitimer(ITIMER_REAL, &new_value, NULL);//非阻塞
    printf("timer begin\n");

    if(ret == -1) {
        perror("setitimer");
        return -1;
    }

    while(1){

    }

    return 0;
}

