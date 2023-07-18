/*
    date:2023-7-18
    A program that using stat function to get information of a file.

    int stat(const char *pathname, struct stat *statbuf);
    作用：获取一个文件的相关信息
    参数：
        -pathname： 文件路径
        -statbuf： 结构体变量，传出参数，用于保存获取到的文件的信息

    返回值：
        成功：0
        失败：-1


    struct stat {
               dev_t     st_dev;          ID of device containing file 
               ino_t     st_ino;          Inode number 
               mode_t    st_mode;         File type and mode， 16位
               nlink_t   st_nlink;        Number of hard links 
               uid_t     st_uid;          User ID of owner 
               gid_t     st_gid;          Group ID of owner 
               dev_t     st_rdev;         Device ID (if special file) 
               off_t     st_size;         Total size, in bytes 
               blksize_t st_blksize;      Block size for filesystem I/O 
               blkcnt_t  st_blocks;       Number of 512B blocks allocated 
        }


    int lstat(const char *pathname, struct stat *statbuf);
    作用：获取软连接文件的信息，而不是软连接指向的文件的信息


*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


int main(){
    struct stat statbuf;

    int ret = stat("from.txt", &statbuf);

    if(ret == -1) {
        perror("stat");
        return -1;
    }

    printf("file size : %ld\n", statbuf.st_size);


    return 0;
}