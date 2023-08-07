/*
    date:2023-8-7

    http连接类
*/

#ifndef HTTPCONN_H
#define HTTPCONN_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdarg.h>
#include <sys/uio.h>


class HttpConn {
public:
    HttpConn(){}

    ~HttpConn(){}

    // 处理客户端请求，解析请求报文，由线程池中的工作线程调用
    void process();

    // 将新的客户数据初始化，放到数组中
    void init(int sockfd, const sockaddr_in &addr);

    // 关闭连接
    void close_conn();


    // 非阻塞一次性读完数据
    bool read();

    // 非阻塞一次性写数据
    bool write();
    

    // 所有的socket上的事件都被注册到同一个epoll对象中
    static int m_epollfd;

    // 用户数量
    static int m_user_count;


private:
    int m_sockfd;   //该http连接的socket
    sockaddr_in m_address; //客户端通信的socke地址

};





#endif 