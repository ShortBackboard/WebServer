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
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

int main() {


    return 0;
}
