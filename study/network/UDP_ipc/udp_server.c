/*

    date:2023-8-5


    include <sys/types.h>
    #include <sys/socket.h>

    ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,
                      const struct sockaddr *dest_addr, socklen_t addrlen);
        - 参数：
            - sockfd : 通信的fd
            - buf : 要发送的数据
            - len : 发送数据的长度
            - flags : 一般写0
            - dest_addr : 通信的另外一端的地址信息
            - addrlen : 地址的内存大小
        
        返回值：
            -成功：发生数据的长度
            -失败：-1

                
    ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags,
                        struct sockaddr *src_addr, socklen_t *addrlen);
        - 参数：
            - sockfd : 通信的fd
            - buf : 接收数据的数组
            - len : 数组的大小
            - flags : 一般写0
            - src_addr : 用来保存另外一端的地址信息，不需要可以指定为NULL
            - addrlen : 地址的内存大小
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

int main() {

    // 1.创建一个用于通信的socket
    // 区别：TCP：需要创建一个socket监听客户端
    //      UDP：面向无连接的，发生数据报
    int fd = socket(PF_INET, SOCK_DGRAM, 0);
    
    if(fd == -1) {
        perror("socket");
        exit(-1);
    }   

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9999);
    addr.sin_addr.s_addr = INADDR_ANY;

    // 2.绑定
    int ret = bind(fd, (struct sockaddr *)&addr, sizeof(addr));
    if(ret == -1) {
        perror("bind");
        exit(-1);
    }

    // 3.通信
    while(1) {
        char recvbuf[128];
        char ipbuf[16];

        struct sockaddr_in cliaddr;
        int len = sizeof(cliaddr);

        // 接收数据
        int num = recvfrom(fd, recvbuf, sizeof(recvbuf), 0, (struct sockaddr *)&cliaddr, &len);

        // 输出客户端信息
        printf("client IP : %s, Port : %d\n", 
            inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, ipbuf, sizeof(ipbuf)),
            ntohs(cliaddr.sin_port));

        // 输出接收的数据
        printf("client say : %s\n", recvbuf);

        // 发送数据
        sendto(fd, recvbuf, strlen(recvbuf) + 1, 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));

    }

    close(fd);
    return 0;
}