/*
    date:2023-8-4

    Epoll 的工作模式：

    ET 模式（边沿触发）
    假设委托内核检测读事件 -> 检测fd的读缓冲区
    读缓冲区有数据 - > epoll检测到了会给用户通知
         a.用户不读数据，数据一致在缓冲区中，epoll下次检测的时候就不通知了
         b.用户只读了一部分数据，epoll不通知
         c.缓冲区的数据读完了，不通知

    ET（edge - triggered）是高速工作方式，只支持 no-block socket。在这种模式下，当描述  
    符从未就绪变为就绪时，内核通过epoll告诉你。然后它会假设你知道文件描述符已经就绪，
    并且不会再为那个文件描述符发送更多的就绪通知，直到你做了某些操作导致那个文件描述
    符不再为就绪状态了。但是请注意，如果一直不对这个 fd 作 IO 操作（从而导致它再次变成
    未就绪），内核不会发送更多的通知（only once, 只通知一次）。

    ET 模式在很大程度上减少了 epoll 事件被重复触发的次数，因此效率要比 LT 模式高。epoll 
    工作在 ET 模式的时候，必须使用非阻塞套接口，以避免由于一个文件句柄(fd)的阻塞读/阻塞写
    操作把处理多个文件描述符的任务饿死。
*/


#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>

int main() {

    // 创建socket
    int lfd = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in saddr;
    saddr.sin_port = htons(9999);
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;

    // 绑定
    bind(lfd, (struct sockaddr *)&saddr, sizeof(saddr));

    // 监听
    listen(lfd, 8);

    // 调用epoll_create()创建一个epoll实例
    int epfd = epoll_create(100);

    // 将监听的文件描述符相关的检测信息添加到epoll实例中
    struct epoll_event epev;
    epev.events = EPOLLIN;
    epev.data.fd = lfd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &epev);

    struct epoll_event epevs[1024];

    while(1) {

        int ret = epoll_wait(epfd, epevs, 1024, -1);
        if(ret == -1) {
            perror("epoll_wait");
            exit(-1);
        }

        printf("ret = %d\n", ret);

        for(int i = 0; i < ret; i++) {

            int curfd = epevs[i].data.fd;

            if(curfd == lfd) {
                // 监听的文件描述符有数据达到，有客户端连接
                struct sockaddr_in cliaddr;
                int len = sizeof(cliaddr);
                int cfd = accept(lfd, (struct sockaddr *)&cliaddr, &len);

                // 设置cfd属性非阻塞
                int flag = fcntl(cfd, F_GETFL);
                flag |= O_NONBLOCK;
                fcntl(cfd, F_SETFL, flag);

                epev.events = EPOLLIN | EPOLLET;    // 设置边沿触发,只会通知一次
                epev.data.fd = cfd;
                epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &epev);
            } else {
                if(epevs[i].events & (EPOLLOUT | !EPOLLIN)) {
                    continue;
                }  

                // 循环读取出所有数据
                char buf[5]; //一次只读5个字节
                int len = 0;
                while( (len = read(curfd, buf, sizeof(buf))) > 0) {//一次只读5个字节，一次读不完，所以需要循环读取
                    // 前面已经把read设置成非阻塞，读取完数据后自动跳出while循环
                    // 打印数据
                    // printf("recv data : %s\n", buf);
                    write(STDOUT_FILENO, buf, len);//写到终端
                    write(curfd, buf, len);
                }
                if(len == 0) {
                    printf("client closed....\n");
                }else if(len == -1) {
                    if(errno == EAGAIN) {
                        //EAGAIN The file descriptor fd refers to a file other than a  socket  and  has  been  marked  nonblocking
                        //(O_NONBLOCK), and the read would block.  See open(2) for further details on the O_NONBLOCK flag.
                        //在非阻塞的情况下数据读完了再继续read读会产生的问题,这种情况不需要退出程序
                        printf("data over.....\n");
                    }else {
                        perror("read");
                        exit(-1);
                    }
                    
                }

            }

        }
    }

    close(lfd);
    close(epfd);
    return 0;
}