/*
    date:2023-7-19

    file descriptor

    //重定向文件描述符
    int dup2(int oldfd, int newfd);
    返回值和newfd相同

    如果newfd已经使用，会先关闭指向的文件，再重新指向oldfd指向的文件

    oldfd必须是有效的文件描述符

*/

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main() {
    int fd = open("1.txt", O_RDWR | O_CREAT, 0777);

    if(fd == -1) {
        perror("open");
        return -1;
    }

    int fd1 = open("2.txt", O_RDWR | O_CREAT, 0777);

    if(fd1 == -1) {
        perror("open");
        return -1;
    }

    printf("before dup2 --- fd :%d, fd1 :%d\n", fd, fd1);

    int fd2 = dup2(fd, fd1);

    if(fd2 == -1) {
        perror("dup2");
        return -1;
    }

    printf("after dup2 --- fd :%d, fd1 :%d, fd2 :%d\n", fd, fd1, fd2);
    //fd1 和 fd2 相同

    //写数据
    char *str = "hello,world!\n";
    int ret = write(fd1, str, strlen(str));

    close(fd);
    close(fd1);
    close(fd2);



    return 0;
}