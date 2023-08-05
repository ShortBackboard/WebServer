/*

    date:2023-8-5

    广播，属于UDP通信

    TCP只支持单播

    向子网中多台计算机发送消息，并且子网中所有的计算机都可以接收到发送方发送的消息，每个广
    播消息都包含一个特殊的IP地址，这个IP中子网内主机标志部分的二进制全部为1。
        a.只能在局域网中使用。
        b.客户端需要绑定服务器广播使用的端口，才可以接收到广播消息。

    
    // 设置广播属性的函数
    int setsockopt(int sockfd, int level, int optname,const void *optval, socklen_t 
    optlen);
    
    参数:
        - sockfd : 文件描述符
        - level : SOL_SOCKET
        - optname : SO_BROADCAST
        - optval : int类型的值，为1表示允许广播
        - optlen : optval的大小

*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

int main() {

    // 1.创建一个通信的socket
    int fd = socket(PF_INET, SOCK_DGRAM, 0);
    if(fd == -1) {
        perror("socket");
        exit(-1);
    }   

    // 2.设置广播属性
    int op = 1;
    setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &op, sizeof(op));
    
    // 3.创建一个广播的地址
    struct sockaddr_in cliaddr;
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_port = htons(9999);
    
    // 最后全"1"用于广播
    inet_pton(AF_INET, "192.168.56.255", &cliaddr.sin_addr.s_addr);

    // 3.通信
    int num = 0;
    while(1) {
       
        char sendBuf[128];
        sprintf(sendBuf, "hello, client....%d\n", num++);
        // 发送数据
        sendto(fd, sendBuf, strlen(sendBuf) + 1, 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
        printf("广播的数据：%s\n", sendBuf);
        sleep(1);
    }

    close(fd);
    return 0;
}