/*

    date:2023-8-4

    poll：对select技术的改进

    #include <poll.h>
    struct pollfd {
        int   fd;         // 委托内核检测的文件描述符 
        short events;     // 委托内核检测文件描述符的什么事件 
        short revents;    // 文件描述符实际发生的事件，内核监听到事件后改变，用户态根据这个值进行对应的操作
                          // 事件类型看pdf或者man文档
                          // 写事件很少去监听
    };

    struct pollfd myfd;
    myfd.fd = 5;
    myfd.events = POLLIN | POLLOUT;

    int poll(struct pollfd *fds, nfds_t nfds, int timeout);
        - 参数：
            - fds : 是一个struct pollfd 结构体数组，这是一个需要检测的文件描述符的集合
            - nfds : 这个是第一个参数数组中最后一个有效元素的下标 + 1
            - timeout : 阻塞时长
                0 : 不阻塞
                -1 : 阻塞，当检测到需要检测的文件描述符有变化，解除阻塞
                >0 : 阻塞的时长
        - 返回值：
            -1 : 失败
            >0（n） : 成功,n表示检测到集合中有n个文件描述符发生变化,用户态再遍历寻找
            


    #include <sys/epoll.h>
    // 创建一个新的epoll实例。在内核中创建了一个数据，这个数据中有两个比较重要的数据，一个是需要检
    测的文件描述符的信息（红黑树），还有一个是就绪列表，存放检测到数据发送改变的文件描述符信息（双向
    链表）。
    int epoll_create(int size);
        - 参数：
            size : 目前没有意义了。随便写一个数，必须大于0
        - 返回值：
            -1 : 失败
            > 0 : 文件描述符，操作epoll实例的


*/

#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <poll.h>


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

    // 初始化检测的文件描述符数组
    struct pollfd fds[1024];//大小可以任意指定
    for(int i = 0; i < 1024; i++) {
        fds[i].fd = -1;
        fds[i].events = POLLIN;//监听读事件
    }
    fds[0].fd = lfd;
    int nfds = 0;

    while(1) {

        // 调用poll系统函数，让内核帮检测哪些文件描述符有数据
        int ret = poll(fds, nfds + 1, -1);// -1：阻塞
        if(ret == -1) {
            perror("poll");
            exit(-1);
        } else if(ret == 0) {
            continue;
        } else if(ret > 0) {
            // 说明检测到了有文件描述符的对应的缓冲区的数据发生了改变
            if(fds[0].revents & POLLIN) {
                // 表示有新的客户端连接进来了
                struct sockaddr_in cliaddr;
                int len = sizeof(cliaddr);
                int cfd = accept(lfd, (struct sockaddr *)&cliaddr, &len);

                // 将新的文件描述符加入到集合中
                for(int i = 1; i < 1024; i++) {
                    if(fds[i].fd == -1) {
                        fds[i].fd = cfd;
                        fds[i].events = POLLIN;
                        break;
                    }
                }

                // 更新最大的文件描述符的索引
                nfds = nfds > cfd ? nfds : cfd;
            }

            //遍历寻找returned event为POLLIN的数组元素，开始通信
            for(int i = 1; i <= nfds; i++) {
                if(fds[i].revents & POLLIN) {
                    // 说明这个文件描述符对应的客户端发来了数据
                    char buf[1024] = {0};
                    int len = read(fds[i].fd, buf, sizeof(buf));
                    if(len == -1) {
                        perror("read");
                        exit(-1);
                    } else if(len == 0) {
                        printf("client closed...\n");
                        close(fds[i].fd);
                        fds[i].fd = -1;
                    } else if(len > 0) {
                        printf("read buf = %s\n", buf);
                        write(fds[i].fd, buf, strlen(buf) + 1);
                    }
                }
            }

        }

    }
    close(lfd);
    return 0;
}

