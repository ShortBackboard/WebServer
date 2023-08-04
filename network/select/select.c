/*

    date:2023-8-4

    IO多路复用

    select

    主旨思想：
1. 首先要构造一个关于文件描述符的列表，将要监听的文件描述符添加到该列表中。
2. 调用一个系统函数，监听该列表中的文件描述符，直到这些描述符中的一个或者多个进行I/O
操作时，该函数才返回。
a.这个函数是阻塞
b.函数对文件描述符的检测的操作是由内核完成的
3. 在返回时，它会告诉进程有多少（哪些）描述符要进行I/O操作


    sizeof(fd_set) = 128字节     1024位,刚好是最大文件描述符数量,进行位操作

文件描述符的最大数量1024,意味着文件描述符的取值范围是0~1023,
并且0、1、2已经被分配了,0表示标准输入,1表示标准输出,2表示标准错误输出,

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/select.h>


int select(int nfds, fd_set *readfds, fd_set *writefds,
           fd_set *exceptfds, struct timeval *timeout);
    - 参数：
            - nfds : 委托内核检测的最大文件描述符的值 + 1 (数组下标从0开始)
            - readfds : 要检测的文件描述符的读的集合，委托内核检测哪些文件描述符的读的属性
                      - 一般检测读操作
                      - 对应的是对方发送过来的数据，因为读是被动的接收数据，检测的就是读缓冲
区
                      - 是一个传入传出参数，由用户态拷贝到内核态，内核操作完后由传出给用户态
                      - 有数据到达：置为1， 没有数据到达：置为0，再传出
            - writefds : 要检测的文件描述符的写的集合，委托内核检测哪些文件描述符的写的属性
                      - 委托内核检测写缓冲区是不是还可以写数据（不满的就可以写）
                      - 没有写满：置为1
                      - 一般设置NULL，不监听写的集合
            - exceptfds : 检测发生异常的文件描述符的集合
            - timeout : 设置的超时时间
                struct timeval {
                    long    tv_sec;        // 秒
                    long    tv_usec;       // 微秒
                };
                - NULL : 永久阻塞，直到检测到了文件描述符有变化
                - tv_sec = 0, tv_usec = 0， 不阻塞
                - tv_sec > 0, tv_usec > 0， 阻塞对应的时间
                    
       - 返回值 : 
            - -1 : 失败
            - >0(n) : 检测的集合中有n个文件描述符发生了变化
                只返回发生了变化的个数，需要重新去遍历寻找为1的，然后进行操作


// 将参数文件描述符fd对应的标志位设置为0
void FD_CLR(int fd, fd_set *set);

// 判断fd对应的标志位是0还是1， 返回值 ： fd对应的标志位的值，0，返回0， 1，返回1
int  FD_ISSET(int fd, fd_set *set);

// 将参数文件描述符fd 对应的标志位设置为1
void FD_SET(int fd, fd_set *set);

// fd_set一共有1024 bit, 全部初始化为0
void FD_ZERO(fd_set *set);
*/

#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>

int main() {

    // 创建socket
    int lfd = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in saddr;
    saddr.sin_port = htons(9999);
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;//绑定本机网卡的所有IP

    // 绑定
    bind(lfd, (struct sockaddr *)&saddr, sizeof(saddr));

    // 监听
    listen(lfd, 8);

    // 创建一个fd_set的集合，存放的是需要检测的文件描述符
    fd_set rdset, tmp;
    FD_ZERO(&rdset);
    FD_SET(lfd, &rdset);
    int maxfd = lfd;

    while(1) {
        // 用户对reset操作
        // 内核对tmp操作
        // 每次循环开始时重新赋值tem，让内核监听原有的和新连接的fd
        tmp = rdset;

        // 调用select系统函数，让内核帮检测哪些文件描述符有数据
        int ret = select(maxfd + 1, &tmp, NULL, NULL, NULL);
        
        if(ret == -1) {
            perror("select");
            exit(-1);
        } else if(ret == 0) {
            continue;
        } else if(ret > 0) {
            // 说明检测到了有文件描述符的对应的缓冲区的数据发生了改变
            if(FD_ISSET(lfd, &tmp)) {
                // 表示有新的客户端连接进来了
                struct sockaddr_in cliaddr;
                int len = sizeof(cliaddr);
                int cfd = accept(lfd, (struct sockaddr *)&cliaddr, &len);

                // 将新的文件描述符加入到集合中
                FD_SET(cfd, &rdset);

                // 更新最大的文件描述符
                maxfd = maxfd > cfd ? maxfd : cfd;
            }


            //遍历寻找为1的数组元素，开始通信
            for(int i = lfd + 1; i <= maxfd; i++) {
                if(FD_ISSET(i, &tmp)) {
                    // 说明这个文件描述符对应的客户端发来了数据
                    char buf[1024] = {0};
                    int len = read(i, buf, sizeof(buf));
                    if(len == -1) {
                        perror("read");
                        exit(-1);
                    } else if(len == 0) {
                        printf("client closed...\n");
                        close(i);
                        FD_CLR(i, &rdset);
                    } else if(len > 0) {
                        printf("read buf = %s\n", buf);
                        // 回写
                        write(i, buf, strlen(buf) + 1);
                    }
                }
            }

        }

    }
    close(lfd);
    return 0;
}

