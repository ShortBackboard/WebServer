/*

    date:2023-7-22

    孤儿进程orphan process
    当父进程比子进程先结束，这样的子进程就成为了孤儿进程。
    每当出现一个孤儿进程，内核会把孤儿进程的父进程设置为init(托管)
    init进程会循环wait()孤儿进程结束，然后做剩下的善后，回收资源
    因此，孤儿进程并没有什么危害

    init进程的pid: 1

    命令ps aux:查看所有进程

    运行可执行程序，切换到后台，有输出就输出到前台
    
*/

#include <unistd.h> 
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

int main() {
    int ret = fork();

    if(ret == -1) {
        perror("fork");
        return -1;

    } else if(ret > 0) {
        //父进程
        printf("this is parent process, pid :%d, ppid :%d\n", getpid(), getppid());
        exit(0);
    } else if(ret == 0) {
        //子进程
        sleep(1);
        //让父进程比子进程先结束 
        printf("this is child process, pid :%d, ppid :%d\n", getpid(), getppid());
    }

    return 0;
}