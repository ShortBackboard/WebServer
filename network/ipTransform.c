/*

    2023-8-1

    IP地址转换函数
    点分十进制和int之间的转换

    1.人们习惯可读性好的点分十进制表示IPv4地址以及十六进制字符串表示IPv6地址
    转换成计算机方便使用的二进制数
    2.主机和网络字节序之间的转换


    #include <arpa/inet.h>
    // p:表示point，点分十进制的IP字符串；n:表示network，网络字节序的整数
    //将点分十进制的IP地址字符串转换成网络字节序的整数
    //inet_pton - convert IPv4 and IPv6 addresses from text to binary form

    int inet_pton(int af, const char *src, void *dst);
        af:地址族： AF_INET  AF_INET6
        src:需要转换的点分十进制的IP字符串
        dst:转换后的结果保存在这个里面

    返回值：
    inet_pton()  returns  1  on  success (network address was successfully converted).  0 is returned if src
    does not contain a character string representing a valid network address in the specified  address  fam‐
    ily.  If af does not contain a valid address family, -1 is returned and errno is set to EAFNOSUPPORT.


    
    // 将网络字节序的整数，转换成点分十进制的IP地址字符串
    //inet_ntop - convert IPv4 and IPv6 addresses from binary to text form

    const char *inet_ntop(int af, const void *src, char *dst, socklen_t size);
        af:地址族： AF_INET  AF_INET6
        src: 要转换的ip的整数的地址
        dst: 转换成IP地址字符串保存的地方
        size：第三个参数的大小（数组的大小）
    
    返回值：On success, inet_ntop() returns a non-null pointer to dst.  
    NULL is returned if there was an error, with errno set to indicate the error.
    返回转换后的数据的地址，和 dst 是一样的


*/

#include <stdio.h>
#include <arpa/inet.h>

int main() {

    // 创建一个ip字符串,点分十进制的IP地址字符串
    char buf[] = "192.168.1.4";     //192是高位，一个字节
    unsigned int num = 0;   //四个字节

    // 将点分十进制的IP字符串转换成网络字节序的整数，保存在num中
    // AF_INET:IPv4
    inet_pton(AF_INET, buf, &num);
    // 再以点分十进制的形式输出
    unsigned char * p = (unsigned char *)&num;
    // "大端"形式
    printf("%d.%d.%d.%d\n\n\n", *p, *(p+1), *(p+2), *(p+3));


    // 将网络字节序的IP整数转换成点分十进制的IP字符串
    char ip[16] = "";   //ip地址点分十进制每一位最多3位(0~255)，3个点3位，再加字符串结束符1位，一共16位
    //结果保存在ip数组中
    const char *str =  inet_ntop(AF_INET, &num, ip, sizeof(ip));
    printf("str : %s\n", str);
    printf("ip : %s\n", ip);
    printf("%d\n", ip == str);//ip和str 指向同一个地方

    return 0;
}