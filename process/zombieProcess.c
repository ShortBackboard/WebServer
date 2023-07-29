/*
    date:2023-7-22

    僵尸进程zombie process
    子进程终止时，父进程尚未回收子进程残留的内核区资源，从而变成僵尸进程

    僵尸进程不能被 kill -9 杀死

    如果父进程不调用wait()或waitpid()的话
    僵尸进程不会被处理，其pcb，pid等资源一直被占用
    这些资源是有限的
    若出现大量的僵尸进程，可能会出现没有可用的pcb资源等
    系统不能创建新的进程

    应当避免僵尸进程的出现
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
        //父进程一直循环，没有去回收子进程资源，子进程变成了僵尸进程
        //可使用ps aux命令去查看进程
        while (1)
        {
            printf("this is parent process, pid :%d, ppid :%d\n", getpid(), getppid());
            sleep(3);
        }
        
        
    } else if(ret == 0) {
        //子进程
        printf("this is child process, pid :%d, ppid :%d\n", getpid(), getppid());
    }

    return 0;
}