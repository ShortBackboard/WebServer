/*

    date:2023-8-5

    本地套接字通信

    本地套接字的作用：本地的进程间通信
    有关系的进程间的通信
    没有关系的进程间的通信

    本地套接字实现流程和网络套接字类似，一般呢采用TCP的通信流程

    // 本地套接字通信的流程 - tcp
    // 服务器端
    1. 创建监听的套接字
        int lfd = socket(AF_UNIX/AF_LOCAL, SOCK_STREAM, 0);
    2. 监听的套接字绑定本地的套接字文件 -> server端
        struct sockaddr_un addr;
        // 绑定成功之后，指定的sun_path中的套接字文件会自动生成。
        bind(lfd, addr, len);
    3. 监听
        listen(lfd, 100);
    4. 等待并接受连接请求
        struct sockaddr_un cliaddr;
        int cfd = accept(lfd, &cliaddr, len);
    5. 通信
        接收数据：read/recv
        发送数据：write/send
    6. 关闭连接
    close();


    // 客户端的流程
    1. 创建通信的套接字
        int fd = socket(AF_UNIX/AF_LOCAL, SOCK_STREAM, 0);
    2. 监听的套接字绑定本地的IP 端口
        struct sockaddr_un addr;
        // 绑定成功之后，指定的sun_path中的套接字文件会自动生成。
        bind(lfd, addr, len);
    3. 连接服务器
        struct sockaddr_un serveraddr;
        connect(fd, &serveraddr, sizeof(serveraddr));
    4. 通信
        接收数据：read/recv
        发送数据：write/send
    5. 关闭连接
        close();


    // 头文件:  sys/un.h
    #define UNIX_PATH_MAX 108
    struct sockaddr_un {
        sa_family_t sun_family; // 地址族协议 af_local
        char sun_path[UNIX_PATH_MAX];   // 套接字文件的路径, 这是一个伪文件, 大小永远=0
    };


*/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/un.h>

int main() {
    // 如果有，首先删除该套接字文件
    unlink("server.sock");

    // 1.创建监听的套接字
    int lfd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if(lfd == -1) {
        perror("socket");
        exit(-1);
    }

    // 2.绑定本地套接字文件
    struct sockaddr_un addr;
    addr.sun_family = AF_LOCAL;
    // 会生成套接字文件
    strcpy(addr.sun_path, "server.sock");
    int ret = bind(lfd, (struct sockaddr *)&addr, sizeof(addr));
    if(ret == -1) {
        perror("bind");
        exit(-1);
    }

    // 3.监听
    ret = listen(lfd, 100);
    if(ret == -1) {
        perror("listen");
        exit(-1);
    }

    // 4.等待客户端连接
    struct sockaddr_un cliaddr;
    int len = sizeof(cliaddr);
    int cfd = accept(lfd, (struct sockaddr *)&cliaddr, &len);
    if(cfd == -1) {
        perror("accept");
        exit(-1);
    }

    printf("client socket filename: %s\n", cliaddr.sun_path);

    // 5.通信
    while(1) {
        // 接收数据
        char buf[128];
        int len = recv(cfd, buf, sizeof(buf), 0);

        if(len == -1) {
            perror("recv");
            exit(-1);
        } else if(len == 0) {
            printf("client closed....\n");
            break;
        } else if(len > 0) {
            // 发送数据
            printf("client say : %s\n", buf);
            send(cfd, buf, len, 0);
        }

    }

    close(cfd);
    close(lfd);

    return 0;
}