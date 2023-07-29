/*
    date:2023-7-19

    int fcntl(int fd, int cmd, ...  arg);

    cmd：command, 表示对fd文件描述符如何操作
    ...:可以没有也可以多个参数 


    作用：1.复制文件描述符 F_DUPFD, 返回得到一个新的文件描述符，分配最小的未使用的
          2.获取或者修改文件描述符标记flags
            F_GETFL: 获取指定的文件描述符的文件状态flags
            F_SETFL: 设置指定的文件描述符的文件状态flags
          
          等其他作用(共5个)


    flags：O_RDONLY，O_WRONLY，O_RDWR, O_CREATE 不可修改
           
           可改：
           O_APPEND: 追加写
           O_NONBLOCK: 设置成非阻塞
           


    
*/

#include <unistd.h> 
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>


int main() {
    // 1.复制文件描述符
    int fd = open("from.txt", O_RDWR);

    if(fd == -1) {
        perror("open");
        return -1;
    }

    // 复制
    int fd1 = fcntl(fd, F_DUPFD);

    if(fd1 == -1) {
        perror("dup");
        return -1;
    }

    printf("Fcntl dup, fd :%d, fd1 :%d\n", fd, fd1);


    // 2.获取或者修改文件描述符标记flags

    // 2.1 先获取文件的描述符标记
    int flag = fcntl(fd, F_GETFL);

    // 2.2 修改添加新的描述符标记
    flag |= O_APPEND;

    int ret = fcntl(fd, F_SETFL, flag);

    if(ret == -1) {
        perror("modify permissions");
        return -1;
    }

    // 2.3追加写内容
    char *str = "append write\n\n";
    write(fd, str, strlen(str));

    close(fd);
    close(fd1);

    return 0;
}