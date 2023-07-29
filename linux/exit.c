/*

    date:2023-7-22

    子进程结束释放自己的用户区数据，内核区数据由其父进程回收释放；(pcb,fd,等)


    //进程退出函数
    void exit(int status);
    -status: 是进程退出时的一个状态信息，父进程在回收子进程资源时可以获取

*/

#include <unistd.h> 
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

int main() {
    printf("hello\n");// \n 自动刷新缓冲区，打印数据
    printf("world");

    //exit(0);//退出进程前会刷新缓冲区，打印world
    _exit(0);//系统提供的函数，不刷新缓冲区, 直接退出进程，不会打印world


    return 0;
}