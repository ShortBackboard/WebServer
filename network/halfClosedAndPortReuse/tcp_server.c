/*

    date:2023-8-3

    查看socke连接使用状态信息命令
    netstat -anp | grep 9999


    半关闭状态：只能接收数据，不能主动发送数据

    #include <sys/socket.h>
        int shutdown(int sockfd, int how);
            sockfd: 需要关闭的socket的描述符
            how:    允许为shutdown操作选择以下几种方式:
                SHUT_RD(0)： 关闭sockfd上的读功能，此选项将不允许sockfd进行读操作。
                    该套接字不再接收数据，任何当前在套接字接受缓冲区的数据将被无声的丢弃掉。
                SHUT_WR(1):  关闭sockfd的写功能，此选项将不允许sockfd进行写操作。进程不能在对此套接字发
                    出写操作。
                SHUT_RDWR(2):关闭sockfd的读写功能。相当于调用shutdown两次：首先是以SHUT_RD,然后以
                    SHUT_WR。



    端口复用最常用的用途是:
        1.防止服务器重启时之前绑定的端口还未释放
        
        2.程序突然退出而系统没有释放端口

        #include <sys/types.h> 
        #include <sys/socket.h>
        // 设置套接字的属性（不仅仅能设置端口复用）
        int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t 
optlen);
    参数：
        - sockfd : 要操作的文件描述符
        - level : 级别 - SOL_SOCKET (端口复用的级别)
        - optname : 选项的名称
            - SO_REUSEADDR
            - SO_REUSEPORT
        - optval : 端口复用的值（整形）
            - 1 : 可以复用
            - 0 : 不可以复用
        - optlen : optval参数的大小


    端口复用，设置的时机是在服务器绑定端口之前。
    setsockopt();
    bind();





*/

#include <stdio.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {

    // 创建socket
    int lfd = socket(PF_INET, SOCK_STREAM, 0);

    if(lfd == -1) {
        perror("socket");
        return -1;
    }

    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;
    saddr.sin_port = htons(9999);

    //端口复用
    //int optval = 1;
    //setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    int optval = 1;
    setsockopt(lfd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));

    // 绑定
    int ret = bind(lfd, (struct sockaddr *)&saddr, sizeof(saddr));
    if(ret == -1) {
        perror("bind");
        return -1;
    }

    // 监听
    ret = listen(lfd, 8);
    if(ret == -1) {
        perror("listen");
        return -1;
    }

    // 接收客户端连接
    struct sockaddr_in cliaddr;
    socklen_t len = sizeof(cliaddr);
    int cfd = accept(lfd, (struct sockaddr *)&cliaddr, &len);
    if(cfd == -1) {
        perror("accpet");
        return -1;
    }

    // 获取客户端信息
    char cliIp[16];
    inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, cliIp, sizeof(cliIp));
    unsigned short cliPort = ntohs(cliaddr.sin_port);

    // 输出客户端的信息
    printf("client's ip is %s, and port is %d\n", cliIp, cliPort );

    // 接收客户端发来的数据
    char recvBuf[1024] = {0};
    while(1) {
        int len = recv(cfd, recvBuf, sizeof(recvBuf), 0);
        if(len == -1) {
            perror("recv");
            return -1;
        } else if(len == 0) {
            printf("客户端已经断开连接...\n");
            break;
        } else if(len > 0) {
            printf("read buf = %s\n", recvBuf);
        }

        // 小写转大写
        for(int i = 0; i < len; ++i) {
            recvBuf[i] = toupper(recvBuf[i]);
        }

        printf("after buf = %s\n", recvBuf);

        // 大写字符串发给客户端
        ret = send(cfd, recvBuf, strlen(recvBuf) + 1, 0);
        if(ret == -1) {
            perror("send");
            return -1;
        }
    }
    
    close(cfd);
    close(lfd);

    return 0;
}
