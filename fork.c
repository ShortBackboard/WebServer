/*
    date:2023-7-20

    创建进程
    一次fork，两次执行

    pid_t fork(void);
        返回值：
            0:子进程
            在父进程中返回子进程的id
            -1:失败,在父进程中返回

    通过fork的返回值区分子父进程

    sleep(1); 休眠1秒

*/


#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
    pid_t ret = fork();//pid_t:int

    if(ret == -1) {
        perror("fork");
        return -1;
    }else if(ret == 0) {
        printf("This is child process, pid :%d, ppid :%d.\n", getpid(), getppid());
    }else if(ret > 0) {
        printf("This is parent process, pid :%d, ppid :%d.\n", getpid(), getppid());
    }

    return 0;
}