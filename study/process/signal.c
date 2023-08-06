/*
    date:2023-7-26

    信号捕捉，信号处理

    typedef void (*sighandler_t)(int);

    sighandler_t signal(int signum, sighandler_t handler);
    设置某个信号的捕捉后行为


    signal() sets the disposition of the signal signum to handler, which is either SIG_IGN,  SIG_DFL,  or
    the address of a programmer-defined function (a "signal handler").

    参数：
    -signum：要捕捉的信号

    -handler：如何处理
    If the signal signum is delivered to the process, then one of the following happens:

       *  If the disposition is set to SIG_IGN, then the signal is ignored.

       *  If the disposition is set to SIG_DFL, then the default action associated with the signal (see sig‐
          nal(7)) occurs.

       *  If the disposition is set to a function, then first either the disposition is reset to SIG_DFL, or
          the  signal  is  blocked (see Portability below), and then handler is called with argument signum.
          If invocation of the handler caused the signal to be blocked, then the signal  is  unblocked  upon
          return from the handler.(回调函数:信号捕捉到以后，内核去调用的函数)

    The signals SIGKILL and SIGSTOP cannot be caught or ignored !!!


    RETURN VALUE
       signal()  returns  the previous value of the signal handler, or SIG_ERR on error.  
       In the event of an error, errno is set to indicate the cause.
       第一次返回NULL


*/

#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

//自定义回调函数
void myalarm(int num) {
    printf("timer handler \n");
    printf("The number of the captured signal :%d\n\n", num);
}

// 过3秒以后，每隔2秒定时一次，并设置信号处理器
int main() {
    // //1.设置信号处理,忽略
    // signal(SIGALRM, SIG_IGN);

    //2.设置回调函数
    //void (*sighandler_t)(int); 函数指针
    if(SIG_ERR == signal(SIGALRM, myalarm)) {
        perror("signal");
        return -1;
    }


    struct itimerval new_value;

    //设置间隔时间
    new_value.it_interval.tv_sec = 2;
    new_value.it_interval.tv_usec = 0;

    //设置延迟时间
    new_value.it_value.tv_sec = 3;
    new_value.it_value.tv_usec = 0;
    
    //到时会发送SIGALRM信号,默认处理是结束进程
    int ret = setitimer(ITIMER_REAL, &new_value, NULL);//非阻塞
    printf("timer begin\n\n");

    if(ret == -1) {
        perror("setitimer");
        return -1;
    }

    while(1){

    }

    return 0;
}