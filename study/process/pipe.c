/*
    date:2023-7-23

    匿名管道，又称无名管道、管道
    fork时共享了文件描述符表

    //子进程发数据给父进程，父进程输出

    int pipe(int pipefd[2]);
    功能:创建一个匿名管道用来进程间通信
    半双工
    
    参数：pipefd[2]数组，传出参数
    pipefd[0]：管道的读端
    pipefd[1]：管道的写端
    //先写后读

    可以使用close()来关闭读端或者写端

    返回值：
        成功：0
        失败：-1

    注意：1.匿名管道只能用于有关系的进程间通信
          一次性操作，数据一旦被读走就被管道抛弃
         2.如果管道为空，read阻塞
           如果管道满，write阻塞


    获取匿名管道缓冲的大小：1.ulimit -a
                            2.long pathconf(const char *path, int name);



管道的读写特点：
使用管道时，需要注意以下几种特殊的情况（假设都是阻塞I/O操作）
1.所有的指向管道写端的文件描述符都关闭了（管道写端引用计数为0），有进程从管道的读端
读数据，那么管道中剩余的数据被读取以后，再次read会返回0，就像读到文件末尾一样。

2.如果有指向管道写端的文件描述符没有关闭（管道的写端引用计数大于0），而持有管道写端的进程
也没有往管道中写数据，这个时候有进程从管道中读取数据，那么管道中剩余的数据被读取后，
再次read会阻塞，直到管道中有数据可以读了才读取数据并返回。

3.如果所有指向管道读端的文件描述符都关闭了（管道的读端引用计数为0），这个时候有进程
向管道中写数据，那么该进程会收到一个信号SIGPIPE, 通常会导致进程异常终止。

4.如果有指向管道读端的文件描述符没有关闭（管道的读端引用计数大于0），而持有管道读端的进程
也没有从管道中读数据，这时有进程向管道中写数据，那么在管道被写满的时候再次write会阻塞，
直到管道中有空位置才能再次写入数据并返回。

总结：
    读管道：
        管道中有数据，read返回实际读到的字节数。
        管道中无数据：
            写端被全部关闭，read返回0（相当于读到文件的末尾）
            写端没有完全关闭，read阻塞等待

    写管道：
        管道读端全部被关闭，进程异常终止（进程收到SIGPIPE信号）
        管道读端没有全部关闭：
            管道已满，write阻塞
            管道没有满，write将数据写入，并返回实际写入的字节数
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