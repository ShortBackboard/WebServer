/*
    date:2023-7-22

    pid_t waitpid(pid_t pid, int *wstatus, int options);
    功能：1.回收指定进程号的子进程
         2.设置非阻塞，默认为阻塞

    参数：
        -pid:
            > 0： 回收当前进程组的某个子进程的id
            0: 回收当前进程组的所有子进程
            -1: 回收任意进程组的子进程，waitpid()一次回收一个进程资源(最常用)
            < -1:回收某个进程组的组id的所有子进程,相对的pgid的绝对值加负号

        -options:
            设置阻塞或者非阻塞
            0: 阻塞
            WNOGANG: 非阻塞

    返回值：
        >0: 返回回收的子进程的id
         0: 在非阻塞options下，表示还有子进程活着
        -1: 错误 或 所有子进程死掉


        
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
    int pid = fork();

    if(pid == -1) {
        perror("fork");
        return -1;
    }else if(pid > 0) {
        //父进程
        while(1) {
            sleep(1);
            
            printf("parent, pid :%d\n", getpid());

            int s;
            // int ret = waitpid(-1, &s, 0);//阻塞
            int ret = waitpid(-1, &s, WNOHANG);//非阻塞

            if(ret == -1) {
                printf("all child process died\n");
                break;
            }

            if(ret > 0) {
                if(WIFSIGNALED(s)) {
                //是不是异常终止，比如被发送的信号关闭
                printf("被哪个信号关闭: %d\n", WTERMSIG(s));
                }
            }

            if(ret == 0) {
                //还有子进程活着
                continue;//下面的代码不执行，回到while重新开始
            }
            
            
        }

    }else if(pid == 0) {
        //子进程
        while(1) {
            printf("child,pid :%d\n", getpid());
            sleep(1);
        }
    }

    return 0;
}