/*
    date:2023-7-23

    使用dup2() 重定向标准输出
    将printf的显示的数据写到文件中，不在终端显示
*/

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main() {
    int fd = open("text.txt",O_RDWR);

    //重定向
    int fd2 = dup2(fd, STDOUT_FILENO);

    char buf[1024] = "hello,world\n";

    write(fd, buf, sizeof(buf));

    //此时终端不会有输出
    printf("%s", buf);
    
    close(fd);

    return 0;
}