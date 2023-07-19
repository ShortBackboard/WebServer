/*
    date:2023-7-19


    A program that get the number of regular file of some directory.
    目录遍历相关函数

    // 打开目录
    DIR *opendir(const char *name);
    返回一个指向目录流开头的DIR指针
        错误返回NULL



    // 读取目录中的文件数据
    struct dirent *readdir(DIR *dirp);
    调用一次， 指针移动一次
    返回值保存读取到的文件目录相关信息
    struct dirent {
               ino_t          d_ino;        //Inode number 
               off_t          d_off;        //Not an offset; see below 
               unsigned short d_reclen;     //Length of this record 
               unsigned char  d_type;       //Type of file; not supported
                                            by all filesystem types 
               char           d_name[256];  //Null-terminated filename 
           };


    文件类型d_type： 
              DT_BLK      This is a block device.

              DT_CHR      This is a character device.

              DT_DIR      This is a directory.

              DT_FIFO     This is a named pipe (FIFO).

              DT_LNK      This is a symbolic link.

              DT_REG      This is a regular file.

              DT_SOCK     This is a UNIX domain socket.

              DT_UNKNOWN  The file type could not be determined.

    读取目录流末尾或者读取失败，返回NULL


    // 关闭目录
    int closedir(DIR *dirp);


*/

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

//递归调用
int getFileNum(const char *path);

int main(int argc, char *argv[]) {
    if(argc < 2) {
        printf("%s path\n", argv[0]);
        exit(0);
    }

    int num = getFileNum(argv[1]);

    if(num < 0 ) {
        return -1;
    }

    printf("the number of regular file of %s is %d\n", argv[1], num);

    return 0;
}


int getFileNum(const char *path) {
    // 1.打开目录
    DIR *dir = opendir(path);

    if(dir == NULL ){
        perror("opendir");
        return -1;
    }

    //保存返回值
    int total = 0;

    struct dirent *ptr;

    while((ptr = readdir(dir)) != NULL) {
        //获取名称
        char *dname = ptr->d_name;

        //忽略.和..
        if(strcmp(dname, ".") == 0 || strcmp(dname, "..") == 0) {
            continue;
        }

        //判断是目录还是普通文件

        //目录
        if(ptr->d_type == DT_DIR) {
            char newPath[256];
            sprintf(newPath,"%s/%s", path, dname);
            total += getFileNum(newPath);
        }

        //普通文件
        if(ptr->d_type == DT_REG) {
            total ++;
        }
    }

    //关闭目录
    closedir(dir);

    return total;

    
}