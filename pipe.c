/*
    date:2023-7-23

    匿名管道，又称无名管道、管道

    //子进程发数据给父进程，父进程输出

    int pipe(int pipefd[2]);
    功能:创建一个匿名管道用来进程间通信
    半双工
    
    参数：pipefd[2]数组，传出参数
    pipefd[0]：管道的读端
    pipefd[1]：管道的写端
    //先写后读

    返回值：
        成功：0
        失败：-1

    注意：1.匿名管道只能用于有关系的进程间通信
          一次性操作，数据一旦被读走就被管道抛弃
         2.如果管道为空，read阻塞
           如果管道满，write阻塞


    获取匿名管道缓冲的大小：1.ulimit -a
                            2.long pathconf(const char *path, int name);

*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>



int main() {
    //在fork之前创建匿名管道
    int pipefd[2];

    int ret = pipe(pipefd);

    if(ret == -1) {
        perror("pipe");
        return -1;
    }

    //创建子进程
    int pid = fork();

    if(pid == -1) {
        perror("fork");
        return -1;

    }else if(pid > 0) {
        //父进程
        //读数据
        char buf[1024] = {0};
        read(pipefd[0], buf, sizeof(buf));
        printf("parent receive data: %s, pid :%d\n", buf, getpid());

    }else if(pid == 0) {
        //子进程
        char buf[1024] = "hello, i am child process";
        sleep(3);//会阻塞父进程read
        write(pipefd[1], buf, sizeof(buf));


    }
    

}