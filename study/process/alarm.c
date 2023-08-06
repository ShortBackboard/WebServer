/*
    date:2023-7-25

    unsigned int alarm(unsigned int seconds);
    功能：设置定时器，倒计时结束后给当前进程发送SIGALARM信号

    seconds:倒计时秒数，为0则定时器无效
    取消一个定时器：通过alarm(0);

    SIGALARM:终止当前的进程，每一个进程仅只能有一个的定时器，重复声明会覆盖

    返回值：
        之前没有定时器：返回0
        之前有定时器：返回前一个定时器剩余的秒数
    

    alarm不阻塞
    定时器与进程状态无关，无论什么状态都在计时
    
*/

#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    int ret = alarm(5);
    printf("seconds = %d\n", ret);//0

    sleep(2);
    ret = alarm(5);
    printf("seconds = %d\n", ret);//3

    while (1)
    {
    }
    
    return 0;
}