/*
    date:2023-7-17
    program: use open func to open a file that exists.
    

    //打开一个已经存在的文件
    int open(const char *pathname, int flags);
        -pathname：文件路径；

        -flags： 文件操作权限 
        O_RDONLY，O_WRONLY，O_RDWR  互斥，必选

    返回的值为文件描述符，用于定位磁盘上的一个文件。错误则返回-1；
   
    errno：记录错误信息

    #include <stdio.h>
    void perror(const char *s);
    用于打印errno对于的错误描述


    
    //创建一个新的文件
    int open(const char *pathname, int flags, mode_t mode);
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdio.h>//perror

#include <unistd.h>//close


//打开一个已经存在的文件
int main(){

    //open file
    int fd = open("test.txt",O_RDONLY);
    //    int fd = open("a.txt",O_RDONLY);//not exist

    //error
    if(fd == -1) {
        perror("open error");
    } else {
        printf("open successfully.\n");
    }

    //close flie
    close(fd);

    return 0;
}

