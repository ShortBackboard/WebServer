/*
    date:2023-7-17
    program: use open func to create a file that not exists.
    

    //打开一个已经存在的文件
    int open(const char *pathname, int flags);

    
    //创建一个新的文件
    int open(const char *pathname, int flags, mode_t mode);
    -pathname：要创建的文件路径和文件名；

    -flags： 对文件的操作权限
    O_RDONLY，O_WRONLY，O_RDWR  互斥，必选
    
    可选：O_APPEND: 追加写的方式
          O_CREATE: 文件不存在，则创建新文件



    -mode：八进制的数，表示对创建出的文件的操作权限 eg：0777
    0开头：八进制数

    最终的权限是: mode & ~umask
    按位&：0和任何数都为0

    umask：在创建文件时抹去某些权限，让创建的文件的权限更合理

    不同用户的umask不同，可以修改，通过umask命令查看
    eg：掩码为0002：抹去了其他用户组写的权利
        掩码为022：抹去了当前用户所在组和其他用户组的写的权利

    eg：命令umask022，修改当前终端的umask值


*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdio.h>//perror

#include <unistd.h>//close

//使用open创建一个新文件
int main(){
    //创建一个新的文件
    int fd = open("create.txt", O_RDWR | O_CREAT, 0777);

    if(fd == -1) {
        perror("create file error");
    }else {
        printf("create a new file successfully.\n");
    }

    close(fd);

    return 0;
}