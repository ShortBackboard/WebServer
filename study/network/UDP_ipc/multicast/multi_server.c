/*

    date:2023-8-5

    组播，也称多播

    单播地址标识单个 IP 接口，广播地址标识某个子网的所有 IP 接口，多播地址标识一组 IP 接口。
    单播和广播是寻址方案的两个极端（要么单个要么全部），多播则意在两者之间提供一种折中方
    案。多播数据报只应该由对它感兴趣的接口接收，也就是说由运行相应多播会话应用系统的主机上
    的接口接收。另外，广播一般局限于局域网内使用，而多播则既可以用于局域网，也可以跨广域网
    使用。
        a.组播既可以用于局域网，也可以用于广域网
        b.客户端需要加入多播组，才能接收到多播的数据

    
    
    组播地址
    IP 多播通信必须依赖于 IP 多播地址，在 IPv4 中它的范围从  224.0.0.0  到 239.255.255.255 ，
    并被划分为局部链接多播地址、预留多播地址和管理权限多播地址三类

    224.0.0.0~224.0.0.255           局部链接多播地址：是为路由协议和其它用途保留的地址，路由
                                    器并不转发属于此范围的IP包
    
    224.0.1.0~224.0.1.255           预留多播地址：公用组播地址，可用于Internet；使用前需要申请
    
    224.0.2.0~238.255.255.255       预留多播地址：用户可用组播地址(临时组地址)，全网范围内有效
    
    239.0.0.0~239.255.255.255       本地管理组播地址，可供组织内部使用，类似于私有 IP 地址，不
                                    能用于 Internet，可限制多播范围




    //设置组播
    int setsockopt(int sockfd, int level, int optname,const void *optval, 
    socklen_t optlen);
    
    // 服务器设置多播的信息，指定外出接口
    - level : IPPROTO_IP
    - optname : IP_MULTICAST_IF
    - optval : struct in_addr
    
    // 客户端加入到多播组：
    - level : IPPROTO_IP
    - optname : IP_ADD_MEMBERSHIP
    - optval : struct ip_mreq

    struct ip_mreq
    {
        // IP multicast address of group.  
        struct in_addr imr_multiaddr;   // 组播的IP地址
        // Local IP address of interface.  
        struct in_addr imr_interface;   // 本地的IP地址
    };
    
    typedef uint32_t in_addr_t;
    
    struct in_addr
    {
    in_addr_t s_addr;
    };



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

    // 2.设置多播的属性，设置外出接口
    struct in_addr imr_multiaddr;
    // 初始化多播地址
    inet_pton(AF_INET, "239.0.0.10", &imr_multiaddr.s_addr);
    setsockopt(fd, IPPROTO_IP, IP_MULTICAST_IF, &imr_multiaddr, sizeof(imr_multiaddr));
    
    // 3.初始化客户端的地址信息
    struct sockaddr_in cliaddr;
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_port = htons(9999);
    inet_pton(AF_INET, "239.0.0.10", &cliaddr.sin_addr.s_addr);

    // 3.通信
    int num = 0;
    while(1) {
       
        char sendBuf[128];
        sprintf(sendBuf, "hello, client....%d\n", num++);
        // 发送数据
        sendto(fd, sendBuf, strlen(sendBuf) + 1, 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
        printf("组播的数据：%s\n", sendBuf);
        sleep(1);
    }

    close(fd);
    return 0;
}