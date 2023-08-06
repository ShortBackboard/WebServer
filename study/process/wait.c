/*
    date:2023-7-22

    父进程可以调用wait或者waitpid得到子进程的退出状态，(eixt()返回的状态信息)
    并同时彻底释放子进程的资源，主要是释放子进程内核区的资源，
    子进程用户区资源已由其直接释放

    wait()和waitpid()函数的功能一样
    区别：
        wait()函数会阻塞
        waitpid()默认阻塞，可以设置不阻塞
        waitpid()还可以指定等待哪个子进程结束,然后回收该子进程

    注意：一次wait()或waitpid()函数调用只能清理一个子进程，多个可以使用循环调用


    pid_t wait(int *wstatus);
    -wstatus：进程退出时的状态信息，传入的是int类型的地址，传出参数
              可以为NULL

    退出信息相关宏函数:
    WIFEXITED(status): 非0，进程正常退出;
    WEXITSTATUS(status): 如果以上宏为真，获得进程退出的状态，(exit的参数);


    WIFSIGNALED(status): 非0，进程异常终止；比如被信号终止
    WTERMSIG(status): 如果以上宏为真，获得使进程退出的信号的编号


    WIFSTOPPED(status): 非0，进程处于暂停状态
    WSTOPSIG(status): 如果以上宏为真，获得使进程暂停的信号的编号


    WIFCONTINUED(status): 非0，进程暂停后已经继续运行






    返回值：
        -成功：返回被回收的子进程的id
        -失败：-1（调用失败 或者 所有的子进程都回收了）


    调用wait的进程会阻塞, 直到一个子进程结束或收到信号

    kill -9 + pid : 杀死进程

*/

#include <unistd.h> 
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
    //创建三个子进程
    int pid;

    for(int i = 0; i < 3; i++) {
        pid = fork();
        if(pid == 0) {
            break;
        }
    }

    if(pid > 0) {
        while (1)
        {
        printf("parent, pid: %d\n", getpid());

        //回收子进程
        //未手动杀死子进程时，父进程在此阻塞

        // int ret = wait(NULL);//常用方式
        
        int s;
        int ret = wait(&s);//通过传入的s参数来保存信息

        /////
        if(WIFEXITED(s)) {
            //是不是正常退出
            //获得由exit();的参数信息 
            printf("退出的状态码: %d\n", WEXITSTATUS(s));
        }

        if(WIFSIGNALED(s)) {
            //是不是异常终止，比如被发送的信号关闭
            printf("被哪个信号关闭: %d\n", WTERMSIG(s));
        }
        /////

        if(ret == -1) {
            printf("all child process died.\n");
            break;
        }

        printf("child die, pid :%d\n", ret);

        sleep(2);
        }

    }else if(pid == 0) {
        //子进程
        // while(1) {
        //     //ps aux
        //     //手动通过kill -9 来杀死进程，观察实验结果
        //     //WTERMSIG(s) 获得状态码：9
        //     printf("child, pid: %d\n", getpid());
        //     sleep(2);
        // }
        exit(1);

    }else if(pid = -1) {
        perror("fork");
        return -1;
    }

    return 0;
}