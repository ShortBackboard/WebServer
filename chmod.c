/*
    date:2023-7-19

    int chmod(const char *pathname, mode_t mode);
    作用：修改文件的权限

    参数：
        -mode：需要修改的权限值，宏或者八进制数

    返回值：
        成功 0；
        失败 -1；

*/

#include <sys/stat.h>
#include <stdio.h>

int main() {
    int ret = chmod("test.c", 0777);

    if(ret == -1){
        perror("chmod");
        return -1;
    }

    return 0;
}

       