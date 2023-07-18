/*
    date:2023-7-18
    A program that extend the length of a file using lseek function.

    
    off_t lseek(int fd, off_t offset, int whence);
        -fd: 文件描述符
        -offset: 偏移量，从文件头部开始计算, 单位：字节
        -whence: 
        SEEK_SET
              The file offset is set to offset bytes.
              设置文件指针的偏移量 offset

        SEEK_CUR
              The file offset is set to its current location plus offset bytes.
              设置文件指针的偏移量 offset 加上 当前位置

        SEEK_END
              The file offset is set to the size of the file plus offset bytes.
              设置文件指针的偏移量 offset 加上 文件大小

    返回值：返回文件指针的位置


    作用：1，移动文件指针，可以反复读取一段数据；
            lseek(fd, 0, SEEK_SET);//返回0

          2. 获取当前文件指针的位置
            lseek(fd, 0, SEEK_CUR);

          3. 获取文件长度
            lseek(fd, 0, SEEK_END);

          4. 扩展文件的长度
          eg：扩展100字节
          lseek(fd, 100, SEEK_END);
          注意：需要扩展后写一次数据才能扩展,lseek仅仅是移动文件指针
          
            

*/


#include <unistd.h> //read
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
    int fd = open("from.txt", O_RDWR);

    if(fd == -1) {
        perror("open file");
        return -1;
    }

    //扩展之前的文件长度
    int before = lseek(fd, 0, SEEK_END);
    if(before == -1) {
        perror("before");
        return -1;
    }
    printf("size before extention : %d\n", before);

    //扩展文件长度100字节
    int after = lseek(fd, 100, SEEK_END);
    if(after == -1) {
        perror("after");
        return -1;
    }

    //写入一个空数据
    write(fd, " ", 1);

    after = lseek(fd, 0, SEEK_END);
    printf("size after extention : %d\n", after);

    close(fd);


    return 0;
}