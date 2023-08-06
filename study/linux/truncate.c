/*
    date:2023-7-19

    int truncate(const char *path, off_t length);
    作用：缩小或扩展文件大小至指定的大小


*/

#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

int main() {
    int ret = truncate("test.txt", 90);

    if(ret == -1) {
        perror("truncate");
        return -1;
    }

    return 0;
}
