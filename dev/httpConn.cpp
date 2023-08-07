/*
    date:2023-8-7
*/

#include "httpConn.h"

// 对静态变量初始化
int HttpConn::m_epollfd = -1;
int HttpConn::m_user_count = 0;


// 设置某个文件描述符非阻塞
void setNonblocking(int fd) {
    int old_flag = fcntl(fd, F_GETFL);
    int new_flag = old_flag | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_flag);
}

// 非阻塞一次性读完数据
bool HttpConn::read() {
    printf("一次性读完数据\n");
    return true;
}

// 非阻塞一次性写数据
bool HttpConn::write() {
    printf("一次性写完数据\n");
    return true;
}

// 添加监听的文件描述符相关的检测信息到epoll中
void addfd(int epollfd, int fd, bool one_shot) {
    // event:保存要监听的事件和fd
    struct epoll_event event;
    
    event.data.fd = fd;
    // EPOLLRDHUP：底层解决连接双方一方突然断开的错误处理
    event.events = EPOLLIN | EPOLLRDHUP;  //监听的事件

    // oneshot指的某socket对应的fd事件最多只能被检测一次，不论你设置的是读写还是异常。
    // 因为可能存在这种情况：如果epoll检测到了读事件，数据读完交给一个子线程去处理
    // 如果该线程处理的很慢，在此期间epoll在该socket上又检测到了读事件，
    // 则又给了另一个线程去处理，则在同一时间会存在两个工作线程操作同一个socket。
    // EPOLLONESHOT:防止同一个通信被不同的线程处理
    if(one_shot) {
        event.events |= EPOLLONESHOT;
    }

    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);

    // 设置文件描述符非阻塞
    setNonblocking(fd);
}

// 从epoll中删除文件描述符
void removefd(int epollfd, int fd) {
    epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, 0);
    close(fd);
}

// 修改文件描述符，注意重置EPOLLONESHOT事件，确保下次可读时，EPOLLIN事件触发
void modfd(int epollfd, int fd, int ev) {
    struct epoll_event event;
    
    event.data.fd = fd;
    event.events = ev | EPOLLONESHOT | EPOLLRDHUP;
    epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &event);
}


// 处理客户端请求，解析请求报文，由线程池中的工作线程调用
void HttpConn::process() {
    // 解析http请求
    printf("parse request, create response\n");
    
    // todo:生成响应

}


// 将新的客户数据初始化，放到数组中
void HttpConn::init(int sockfd, const sockaddr_in &addr){
    m_sockfd = sockfd;
    m_address = addr;

    // 设置端口复用
    int reuse{1};
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    // 添加到epoll对象中
    addfd(m_epollfd, sockfd, true);
    m_user_count++;// 总用户数加1
}


// 关闭一个客户端连接
void HttpConn::close_conn() {
    if(m_sockfd != -1) {
        removefd(m_epollfd, m_sockfd);
        m_sockfd = -1;
        m_user_count--;
    }
}


