/*
    date:2023-7-19

    file descriptor
    多个文件描述符可以指向同一个文件


    //复制文件描述符
    int dup(int oldfd);
    
    返回值：
        成功：返回新的最小的未使用的文件描述符
        失败：-1
     
*/

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>


int main() {
    int fd = open("create.txt", O_RDWR | O_CREAT, 0777);

    if(fd == -1) {
        perror("open");
        return -1;
    }

    int fd1 = dup(fd);

    if(fd1 == -1) {
        perror("dup");
        return -1;
    }

    //第一次dup
    printf("1st dup --- fd : %d, fd1 : %d\n", fd, fd1);

    close(fd);

    //写数据
    char *str = "hello,world!\n";
    int ret = write(fd1, str, strlen(str));


    //第二次dup
    int fd2 = dup(fd1);
    printf("2nd dup --- fd1 : %d, fd2 : %d\n", fd1, fd2); 

    close(fd2);
    close(fd1);

    return 0;
}