/*
    date:2023-7-24

    有名管道: FIFO ,namedPipe
    提供了一个路径名相关联，以FIFO文件形式存在于文件系统中
    FIFO的内容存放在内存，文件中没有内容
    创建后可以read、write、close等
    实现了任意不同进程间的通信

    也有读端和写端

    //创建有名管道
    1.命令
    mkfifo

    2.函数
    int mkfifo(const char *pathname, mode_t mode);
    参数：
        -pathname：有名管道的路径
        -mode：和open函数的mode相同

    读端：以仅读的方式打开
    写端：以仅写的方式打开

    返回值：
        成功：0
        失败：-1

    打开有名管道：open函数

    若只打开写端或读端，会阻塞
    
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

int main() {

    // 1.判断有名管道文件是否存在
    int ret = access("fifo1", F_OK);
    if(ret == -1) {
        // 文件不存在
        printf("管道不存在，创建对应的有名管道\n");
        ret = mkfifo("fifo1", 0664);
        if(ret == -1) {
            perror("mkfifo");
            exit(0);
        }
    }

    ret = access("fifo2", F_OK);
    if(ret == -1) {
        // 文件不存在
        printf("管道不存在，创建对应的有名管道\n");
        ret = mkfifo("fifo2", 0664);
        if(ret == -1) {
            perror("mkfifo");
            exit(0);
        }
    }

    // 2.以只写的方式打开管道fifo1
    int fdw = open("fifo1", O_WRONLY);
    if(fdw == -1) {
        perror("open");
        exit(0);
    }
    printf("打开管道fifo1成功，等待写入...\n");
    // 3.以只读的方式打开管道fifo2
    int fdr = open("fifo2", O_RDONLY);
    if(fdr == -1) {
        perror("open");
        exit(0);
    }
    printf("打开管道fifo2成功，等待读取...\n");

    char buf[128];

    // 4.循环的写读数据
    while(1) {
        memset(buf, 0, 128);
        // 获取标准输入的数据
        fgets(buf, 128, stdin);
        // 写数据
        ret = write(fdw, buf, strlen(buf));
        if(ret == -1) {
            perror("write");
            exit(0);
        }

        // 5.读管道数据
        memset(buf, 0, 128);
        ret = read(fdr, buf, 128);
        if(ret <= 0) {
            perror("read");
            break;
        }
        printf("buf: %s\n", buf);
    }

    // 6.关闭文件描述符
    close(fdr);
    close(fdw);

    return 0;
}
