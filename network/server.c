/*

    date:2023-8-2

    // TCP 通信的流程
    // 服务器端 （被动接受连接的角色）
    1. 创建一个用于监听的套接字 (fd)
      - 监听：监听有客户端的连接
       - 套接字：这个套接字其实就是一个文件描述符
    2. 将这个监听文件描述符和本地的IP和端口绑定（IP和端口就是服务器的地址信息）
      - 客户端连接服务器的时候使用的就是这个IP和端口
    3. 设置监听，监听的fd开始工作
    4. 阻塞等待，当有客户端发起连接，解除阻塞，接受客户端的连接，会得到一个和客户端通信的套接字
    （fd）
    5. 通信
           - 接收数据
           - 发送数据
    6. 通信结束，断开连接


    // 客户端
    1. 创建一个用于通信的套接字 (fd)
    2. 连接服务器，需要指定连接的服务器的 IP 和 端口
    3. 连接成功了，客户端可以直接和服务器通信
        - 接收数据
        - 发送数据
    4. 通信结束，断开连接


    //套接字函数


    int socket(int domain, int type, int protocol);
    - 功能：创建一个套接字
    - 参数：
        - domain: 协议族
            AF_INET : ipv4
            AF_INET6 : ipv6
            AF_UNIX, AF_LOCAL : 本地套接字通信（进程间通信）
        - type: 通信过程中使用的协议类型
            SOCK_STREAM : 流式协议 TCP
            SOCK_DGRAM  : 报式协议 UDP
        - protocol : 具体的一个协议。一般写0
            - SOCK_STREAM : 流式协议默认使用 TCP
            - SOCK_DGRAM  : 报式协议默认使用 UDP
        - 返回值：
            - 成功：返回文件描述符，操作的就是内核缓冲区。
            - 失败：-1   


    int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen); 
    // bind 有时又称 socket命名
        - 功能：绑定，将socket()函数返回的 fd 和本地的IP + 端口进行绑定
        - 参数：
            - sockfd : 通过socket()函数得到的文件描述符
            - addr : 需要绑定的socket地址，这个地址封装了ip和端口号的信息
            - addrlen : 第二个参数结构体占的内存大小

    成功 0；失败 -1；
        
    

    int listen(int sockfd, int backlog);    // 系统的最大值cat /proc/sys/net/core/somaxconn 查看
        - 功能：监听这个socket上的连接
        - 参数：
            - sockfd : 通过socket()函数得到的文件描述符
            - backlog : 未连接的和已经连接的和的最大值， 一般指定128就够了
    成功 0；失败 -1；   


    int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
        - 功能：接收客户端连接，默认是一个阻塞的函数，阻塞等待客户端连接 
        - 参数：
            - sockfd : 用于监听的文件描述符
            - addr : 传出参数，记录了连接成功后客户端的地址信息（ip + port）
            - addrlen : 指定第二个参数的对应的内存大小
    - 返回值：
            - 成功 ：用于通信的文件描述符 
            - -1 ： 失败
                

    int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
        - 功能： 客户端调用的函数，客户端连接服务器
        - 参数：
            - sockfd : 用于通信的文件描述符
            - addr : 客户端要连接的服务器的地址信息（ip + port）
            - addrlen : 第二个参数的内存大小
    - 返回值：成功 0， 失败 -1


    ssize_t write(int fd, const void *buf, size_t count);   // 写数据
    ssize_t read(int fd, void *buf, size_t count);          // 读数据


*/

// TCP 通信的服务器端

#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main() {

    // 1.创建socket(用于监听的套接字)
    // listen fd
    // 有客服端请求，就发送到监听fd的读缓冲区,
    // 然后监听fd将系统的最小的未使用的fd与请求端的fd连接进行后续的通信
    int lfd = socket(AF_INET, SOCK_STREAM, 0);//IPv4;流式

    if(lfd == -1) {
        perror("socket");
        exit(-1);
    }

    // 2.绑定
    // TCP/IP 协议族有 sockaddr_in 和 sockaddr_in6 两个专用的 socket 地址结构体，它们分别用于 IPv4 和 IPv6
    struct sockaddr_in saddr; //存储服务器定义的ip + port信息

    saddr.sin_family = AF_INET;
    inet_pton(AF_INET, "192.168.56.101", &saddr.sin_addr.s_addr); //本机的IP地址，实际开发时就是域名ip地址
    //saddr.sin_addr.s_addr = INADDR_ANY;  // 0.0.0.0 表示任意地址
    saddr.sin_port = htons(9999);// 自定义的一个端口号，主机转网络字节序

    int ret = bind(lfd, (struct sockaddr *)&saddr, sizeof(saddr));

    if(ret == -1) {
        perror("bind");
        exit(-1);
    }

    // 3.监听
    ret = listen(lfd, 128);
    if(ret == -1) {
        perror("listen");
        exit(-1);
    }

    // 4.接收客户端连接
    struct sockaddr_in clientaddr;  //记录连接服务器的客服端的信息
    int len = sizeof(clientaddr);
    // client fd
    int cfd = accept(lfd, (struct sockaddr *)&clientaddr, &len);
    
    if(cfd == -1) {
        perror("accept");
        exit(-1);
    }

    // 输出客户端的信息
    // 网络转换成主机字节序
    char clientIP[16];//3*4 + 3 + 1 字符串结束符
    
    // 转换IP
    inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, clientIP, sizeof(clientIP));
    
    // 转换Port
    unsigned short clientPort = ntohs(clientaddr.sin_port);
    printf("client ip is %s, port is %d\n", clientIP, clientPort);


    // 5.通信
    // 一般是服务器先接收数据
    char recvBuf[1024] = {0};
    while(1) {
        
        // 获取客户端的数据
        int num = read(cfd, recvBuf, sizeof(recvBuf));
        if(num == -1) {
            perror("read");
            exit(-1);
        } else if(num > 0) {
            printf("receive client data : %s\n", recvBuf);
        } else if(num == 0) {
            // 表示客户端断开连接
            printf("clinet closed...");
            break;
        }

        char * data = "hello,I am server";
        // 给客户端发送数据
        write(cfd, data, strlen(data));
    }
   
    // 关闭文件描述符
    close(cfd);
    close(lfd);

    return 0;
}

//回射服务器：客服端发送什么，服务器返回什么