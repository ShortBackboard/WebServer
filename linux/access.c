/*
date:2023-7-18


int access(const char *pathname, int mode);
作用：判断当前进程的文件对某个文件是否有某个权限，或判断文件是否存在。
参数：
    -mode：
    F_OK：判断文件是否存在
    R_OK：读
    W_OK：写
    X_OK：执行

返回值：成功 0；
        失败或没有权限 -1；


*/

#include <unistd.h>
#include <stdio.h>

int main(){
    int ret = access("from.txt", F_OK);

    if(ret == -1) {
        perror("access");
        return -1;
    }

    printf("File Exists!\n");

    return 0;
}