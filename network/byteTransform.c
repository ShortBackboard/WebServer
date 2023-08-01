/*

    2023-8-1

    内存是低地址到高地址存储

    网络字节顺序是 TCP/IP 中规定好的一种数据表示格式，它与具体的 CPU 类型、操作系统等无关，从而
    可以保证数据在不同主机之间传输时能够被正确解释
    网络字节顺序采用"大端"排序方式

    网络通信时，需要将主机字节序转换成网络字节序（大端），
    接收方接收到数据以后再将网络字节序转换成本机主机字节序。

    #include <arpa/inet.h>

    BSD Socket提供了封装好的转换接口，方便程序员使用。
    包括从主机字节序到网络字节序的转换函数：htons、htonl

    从网络字节序到主机字节序的转换函数：ntohs、ntohl

    h   - host 主机，主机字节序
    to  - 转换成什么 
    n   - network  网络字节序
    s   - short  unsigned short //uint16_t，两个字节
    l   - long   unsigned int   //uint32_t，四个字节

    //端口：16位
    
    // 转换端口
    uint16_t htons(uint16_t hostshort);		
    uint16_t ntohs(uint16_t netshort);		

    //IP：32位
    // 转换IP
    uint32_t htonl(uint32_t hostlong);		
    uint32_t ntohl(uint32_t netlong);		

*/

#include <stdio.h>
#include <arpa/inet.h>

int main() {
    //本机是"小端"字节序
    //网络通信采用"大端"字节序

    // htons 转换端口,主机的字节序转换成网络的
    unsigned short a = 0x0102;
    printf("a : %x\n", a); //%x:将无符号整数以十六进制形式输出
    unsigned short b = htons(a);
    printf("b : %x\n", b);

    printf("------------------------\n");

    // htonl  转换IP,主机的字节序转换成网络的
    char buf[4] = {192, 168, 1, 100};//100是高位
    int num = *(int *)buf;
    int sum = htonl(num);
    unsigned char *p = (char *)&sum; //char一个字节

    //将100高位放到低地址
    //内存是低地址到高地址存储
    printf("%d.%d.%d.%d\n", *p, *(p+1), *(p+2), *(p+3));

    printf("------------------------\n");

    // ntohl    转换IP,网络的字节序转换成本机的
    unsigned char buf1[4] = {1, 1, 168, 192};
    int num1 = *(int *)buf1;
    int sum1 = ntohl(num1);
    unsigned char *p1 = (unsigned char *)&sum1;
    printf("%d.%d.%d.%d\n", *p1, *(p1+1), *(p1+2), *(p1+3));
    
    printf("------------------------\n");


    // ntohs   转换端口,网络的字节序转换成本机的
    unsigned short c = 0x0201;//从网络收到的大端字节序，01是高位
    unsigned short d = ntohs(c);//把接收到的大端字节序转换成本机的小端字节序
    printf("%x\n", d);


    return 0;
}