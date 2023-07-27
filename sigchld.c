/*

    date:2023-7-27

    
    SIGCHLD信号产生的3个条件：
        1.子进程结束
        2.子进程接收到 SIGSTOP 信号，子进程暂停了
        3.子进程处于暂停态，接收到 SIGCONT 后继续运行
        
        以上三种情况都会给父进程发送该信号，父进程默认忽略该信号。
    
    使用SIGCHLD信号解决僵尸进程的问题。
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/wait.h>

// 捕捉到的信号的编号保存在num中
void myFun(int num) {
    printf("捕捉到的信号 :%d\n", num);
    // 回收子进程PCB的资源

    // 使用waitpid
    while(1) {
       // 设置非阻塞
       int ret = waitpid(-1, NULL, WNOHANG);
       if(ret > 0) {
           printf("child die , pid = %d\n", ret);
       } else if(ret == 0) {
           // 说明还有子进程活着, 父进程跳出循环, 去干自己的事情
           break;
       } else if(ret == -1) {
           // 没有子进程活着
           break;
       }
    }
}

int main() {

    // 提前设置好阻塞信号集，阻塞SIGCHLD，因为有可能子进程很快结束，父进程还没有完成注册信号捕捉
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGCHLD);
    sigprocmask(SIG_BLOCK, &set, NULL);

    // 创建3个子进程
    pid_t pid;
    for(int i = 0; i < 3; i++) {
        pid = fork();
        if(pid == 0) {
            break;
        }
    }

    if(pid > 0) {
        // 父进程

        // 捕捉子进程死亡时发送的SIGCHLD信号
        struct sigaction act;
        act.sa_flags = 0;
        act.sa_handler = myFun;
        sigemptyset(&act.sa_mask);
        sigaction(SIGCHLD, &act, NULL);

        // 注册完信号捕捉以后，解除阻塞，恢复到原来的信号集
        sigprocmask(SIG_UNBLOCK, &set, NULL);

        while(1) {
            printf("parent process pid : %d\n", getpid());
            sleep(2);
        }
    } else if( pid == 0) {
        // 子进程
        printf("child process pid : %d\n", getpid());
    }

    return 0;
}