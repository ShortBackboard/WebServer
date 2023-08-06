/*

    date:2023-8-4

    epoll:event poll

    #include <sys/epoll.h>


    // epoll_create():创建一个新的epoll实例。在内核中创建了一个数据，这个数据中有两个比较重要的数据
    一个是需要检测的文件描述符的信息（红黑树）:
    struct rb_root rbr;//需要监听的fd, red and black

    一个是就绪列表，存放检测到数据发送改变的文件描述符信息（双向链表）:
    struct list_head rblist;//ready

    int epoll_create(int size);//在内核内创建
        - 参数：
            size : 目前没有意义了。随便写一个数，必须大于0
        - 返回值：
            -1 : 失败
            > 0 : 文件描述符，操作epoll实例的


    // 对epoll实例进行管理：添加文件描述符信息，删除信息，修改信息
    int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
        - 参数：
            - epfd : epoll实例对应的文件描述符,epoll_create()的返回值
            - op : 要进行什么操作
                EPOLL_CTL_ADD:  添加
                EPOLL_CTL_MOD:  修改
                EPOLL_CTL_DEL:  删除
            - fd : 要检测的文件描述符
            - event : 检测文件描述符什么事情

        常见的Epoll检测事件：
                - EPOLLIN：读，常用
                - EPOLLOUT：写
                - EPOLLERR：错误

        typedef union epoll_data {
                void        *ptr;
                int          fd;
                uint32_t     u32;
                uint64_t     u64;
            } epoll_data_t;


        struct epoll_event {
            uint32_t     events;      // Epoll events 
            epoll_data_t data;        // User data variable 
        };


    // 检测函数                
    int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int 
    timeout);
        - 参数：
            - epfd : epoll实例对应的文件描述符,epoll_create()的返回值
            - events : 传出参数，保存了发送了变化的文件描述符的信息
            - maxevents : 第二个参数结构体数组的大小
            - timeout : 阻塞时间
                - 0 : 不阻塞
                - -1 : 阻塞，直到检测到fd数据发生变化，解除阻塞
                - > 0 : 阻塞的时长（毫秒）
                
        - 返回值：
            - 成功，返回发送变化的文件描述符的个数 > 0
            - 失败 -1



    Epoll 的工作模式：
    1.
    LT 模式 （水平触发）默认的模式
    假设委托内核检测读事件 -> 检测fd的读缓冲区
    读缓冲区有数据 - > epoll检测到了会给用户通知
        a.用户不读数据，数据一直在缓冲区，epoll 会一直通知
        b.用户只读了一部分数据，epoll会通知
        c.缓冲区的数据读完了，不通知

    LT（level - triggered）是缺省的工作方式，并且同时支持 block 和 no-block socket。在这
    种做法中，内核告诉你一个文件描述符是否就绪了，然后你可以对这个就绪的 fd 进行 IO 操
    作。如果你不作任何操作，内核还是会继续通知你的。如果一次读数据没有读完,后面还是会通知

    2.
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
    // epev:保存要监听的事件和fd
    struct epoll_event epev;
    epev.events = EPOLLIN;  //监听读事件
    epev.data.fd = lfd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &epev);

    //epevs[]:传出数组，保存发生了监听事件的数组，用于用户态操作，大小自定义
    struct epoll_event epevs[1024];

    while(1) {

        int ret = epoll_wait(epfd, epevs, 1024, -1);
        if(ret == -1) {
            perror("epoll_wait");
            exit(-1);
        }

        // ret:监听到发生了事件的个数
        printf("ret = %d\n", ret);

        for(int i = 0; i < ret; i++) {

            int curfd = epevs[i].data.fd;

            if(curfd == lfd) {
                // 监听的文件描述符有数据达到，有客户端连接
                // 将客户端fd添加到epev
                struct sockaddr_in cliaddr;
                int len = sizeof(cliaddr);
                int cfd = accept(lfd, (struct sockaddr *)&cliaddr, &len);

                epev.events = EPOLLIN;
                //  epev.events = EPOLLIN | EPOLLOUT;   //多事件

                epev.data.fd = cfd;
                epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &epev);
            } else {
                // 忽略EPOLLOUT
                // if(epevs[i].events & (EPOLLOUT | !EPOLLIN)) {
                //     continue;
                // } 

                // 本程序是针对的EPOLLIN事件，如果是多事件这里的判断要根据对应的事件宏值更改位运算判断
                // 有数据到达，通信
                char buf[1024] = {0};
                int len = read(curfd, buf, sizeof(buf));
                if(len == -1) {
                    perror("read");
                    exit(-1);
                } else if(len == 0) {
                    printf("client closed...\n");
                    epoll_ctl(epfd, EPOLL_CTL_DEL, curfd, NULL);    //删除
                    close(curfd);
                } else if(len > 0) {
                    printf("read buf = %s\n", buf);
                    write(curfd, buf, strlen(buf) + 1);
                }

            }

        }
    }

    close(lfd);
    close(epfd);
    return 0;
}