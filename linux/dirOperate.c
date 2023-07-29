/*
    date:2023-7-19

    目录操作相关函数

    //创建一个新的空目录
    int mkdir(const char *pathname, mode_t mode);
    权限：mode & ~umask & 0777


    //删除一个空目录
    int rmdir(const char *pathname);


    //对目录进行重命名
    int rename(const char *oldpath, const char *newpath);


    //获得当前的工作路径 (pwd)
    char *getcwd(char *buf, size_t size);
    参数：
        -buf：存储的数组的地址
        -size：数组的大小

    返回值：指向存储的数组的地址


    //修改当前进程所在的工作路径 
    int chdir(const char *path);

*/

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
    // //1.创建一个新的空目录
    // int ret = mkdir("Test", 0774);

    // if(ret == -1) {
    //     perror("mkdir");
    //     return -1;
    // }



    // //2.删除一个空目录
    // int ret = rmdir("Test");
    // if(ret == -1) {
    //     perror("rmdir");
    //     return -1;
    // }


    // //3.对目录进行重命名
    // int ret = rename("Test", "123321");
    // if(ret == -1) {
    //     perror("rename");
    //     return -1;
    // }


    //4.获得当前的工作路径
    char buf[128];
    getcwd(buf, sizeof(buf));
    printf("current file working path is:%s\n",buf);


    //5.修改当前进程所在的工作路径
    int ret = chdir("/home/cnu/");
    if(ret == -1) {
        perror("chdir");
        return -1;
    }

    char buf1[128];
    getcwd(buf1, sizeof(buf1));
    printf("after change, current file working path is:%s\n",buf1);



    return 0;
}