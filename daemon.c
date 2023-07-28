/*
    date:2023-7-28

    进程组：一系列相关进程的集合
    会话：一系列相关进程组的集合
    
    守护进程:daemon process

    守护进程（Daemon Process），也就是通常说的 Daemon 进程（精灵进程），是
    Linux 中的后台服务进程。它是一个生存期较长的进程，通常独立于控制终端并且周
    期性地执行某种任务或等待处理某些发生的事件。一般采用以 d 结尾的名字。

    ◼ 守护进程具备下列特征：
        生命周期很长，守护进程会在系统启动的时候被创建并一直运行直至系统被关闭。
        它在后台运行并且不拥有控制终端。没有控制终端确保了内核永远不会为守护进
        程自动生成任何控制信号以及终端相关的信号（如 SIGINT、SIGQUIT）。
    ◼ Linux 的大多数服务器就是用守护进程实现的。比如，Internet 服务器 inetd，
        Web 服务器 httpd 等。




    守护进程的创建步骤
    ◼ 执行一个 fork()，之后父进程退出，子进程继续执行。防止使用父进程创建会话,sid冲突
    ◼ 子进程调用 setsid() 开启一个新会话。
    ◼ 清除进程的 umask 以确保当守护进程创建文件和目录时拥有所需的权限。
    ◼ 修改进程的当前工作目录，通常会改为根目录（/）。
    ◼ 关闭守护进程从其父进程继承而来的所有打开着的文件描述符。
    ◼ 在关闭了文件描述符0、1、2之后(和终端解除联系)，守护进程通常会打开/dev/null(往这个设备写数据会被自动丢弃),
    并使用dup2(),使所有这些描述符指向这个设备。
    ◼ 核心业务逻辑
*/

/*
    写一个守护进程，每隔2s获取一下系统时间，将这个时间写入到磁盘文件中。
*/

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <signal.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

//捕捉信号回调函数
void work(int num) {
    // 捕捉到信号之后，获取系统时间，写入磁盘文件
    time_t tm = time(NULL);
    //time() returns the time as the number of seconds since the Epoch, 1970-01-01 00:00:00 +0000 (UTC).
    
    //转化成时间格式
    struct tm *loc = localtime(&tm);

    // 方法一
    // char buf[1024];

    // sprintf(buf, "%d-%d-%d %d:%d:%d\n",loc->tm_year,loc->tm_mon
    // ,loc->tm_mday, loc->tm_hour, loc->tm_min, loc->tm_sec);

    // printf("%s\n", buf);


    // 方法二，调用系统函数直接转化
    char *str = asctime(loc);
    int fd = open("time.txt", O_RDWR | O_CREAT | O_APPEND, 0664);
    //写入文件
    write(fd ,str, strlen(str));
    close(fd);
}

int main() {

    // 1.创建子进程
    pid_t pid = fork();
    
    //退出父进程
    if(pid > 0) {
        exit(0);
    }

    // 2.用子进程重新创建一个会话
    // 新的会话会默认脱离原来的控制终端
    setsid();

    // 3.设置掩码
    umask(022);

    // 4.更改工作目录
    chdir("/home/cnu/");

    // 5. 关闭从父进程继承过来的fd、重定向文件描述符
    int fd = open("/dev/null", O_RDWR);
    dup2(fd, STDIN_FILENO); //标准输入
    dup2(fd, STDOUT_FILENO); //标准输出
    dup2(fd, STDERR_FILENO); //标准错误

    // 6.业务逻辑

    // 捕捉定时信号，设置回调函数
    struct sigaction act;
    act.sa_flags = 0;
    act.sa_handler = work;
    sigemptyset(&act.sa_mask);
    sigaction(SIGALRM, &act, NULL);

    //2s以后每隔2s发送一次SIGALRM信号
    struct itimerval val;
    val.it_value.tv_sec = 2;
    val.it_value.tv_usec = 0;
    val.it_interval.tv_sec = 2;
    val.it_interval.tv_usec = 0;

    // 创建定时器
    setitimer(ITIMER_REAL, &val, NULL);

    // 不让进程结束
    while(1) {
        sleep(10);
    }

    return 0;
}