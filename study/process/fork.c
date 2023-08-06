/*
    date:2023-7-20

    创建进程
    一次fork，两次执行

    子进程中不执行fork

    pid_t fork(void);
        返回值：
            0:子进程
            在父进程中返回子进程的id
            -1:失败,在父进程中返回

    通过fork的返回值区分子父进程

    sleep(1); 休眠1秒

    进程组：一个或多个进程的集合，进程组号pgid


    1.父子进程相互独立；
    2.子进程刚被创建，还没有执行任何写操作时，父子进程内的用户区数据、文件描述符等相同，
    读时共享，写时拷贝。

*/


#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
    int num = 10;

    pid_t ret = fork();//pid_t:int

    if(ret == -1) {
        perror("fork");
        return -1;
    }else if(ret == 0) {
        num += 10;
        printf("This is child process, pid :%d, ppid :%d, pgid :%d.\n", getpid(), getppid(), getpgid(getpid()));
        printf("num += 10 :%d\n", num);
    }else if(ret > 0) {
        num += 100;
        printf("This is parent process, pid :%d, ppid :%d, pgid :%d.\n", getpid(), getppid(), getpgid(getpid()));
        printf("num += 100 :%d\n", num);
    }

    return 0;
}