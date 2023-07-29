/*
    date:2023-7-25

    内存映射:memory-mapped IO
    将磁盘文件的数据映射到内存，用户通过修改内存就能修改磁盘文件
    操作内存速度快，效率高
    内存映射是非阻塞的，考虑到读写的顺序

    void *mmap(void *addr, size_t length, int prot, int flags,
                  int fd, off_t offset);
    
    功能：将一个文件或者设备的数据映射到内存中
    
    参数：
        -*addr：开始地址，一般设置为NULL，由内核指定
        length: 要映射的数据的长度，大于0，建议为文件的长度
        prot: 对申请的内存映射区的操作权限，一个或多个
            -PROT_EXEC  Pages may be executed.
            -PROT_READ  Pages may be read.
            -PROT_WRITE Pages may be written.
            -PROT_NONE  Pages may not be accessed.
            有PROT_READ权限才能操作
            open的权限不能和prot权限冲突
            prot的权限不能大于open，建议和open的权限一致
        flags: 
            -MAP_SHARED：映射区的数据自动和磁盘文件进行同步，进行进程间通信
            -MAP_PRIVATE：不同步，写的时候拷贝
        fd: 要映射的那个文件的fd，文件的大小不能为0
            对新创建的文件要truncate
        offset: 偏移量，4K的整数倍；0表示不偏移，从文件头开始操作
    
    返回值：
        成功：内存映射文件的首地址
        失败：MAP_FAILED
    


    //释放内存映射
    int munmap(void *addr, size_t length);
    参数：
        -*addr：*mmap的返回值
        -length：*mmap的length


    匿名映射：prot加上 MAP_ANONYMOUS， fd为-1
        可以实现有关系的进程IPC
        详细见 man mmap

*/

/*
    使用内存映射实现IPC
    1.有关系的进程间
        -在fork前创建
    2.没有关系的进程间
        -需要准备一个大小不为0的磁盘文件

    内存映射区进行IPC非阻塞
*/



#include <sys/mman.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>

int main() {
    //1.打开文件
    int fd = open("mmap.txt", O_RDWR);
    if(fd == -1) {
        perror("open");
        return -1;
    }

    //2.获取文件大小
    int size = lseek(fd, 0, SEEK_END);

    //3.创建内存映射区
    void *ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(ptr == MAP_FAILED) {
        perror("memory mapped");
        return -1;
    }

    //4.创建子进程，进行IPC
    int pid = fork();

    if(pid > 0) {
        //父进程,读数据
        wait(NULL);
        char buf[128];
        strcpy(buf, (char *)ptr);
        printf("receive from son :%s\n", buf);

    }else if(pid == 0) {
        //子进程,写数据
        strcpy((char *)ptr, "i am son!");

    }else if(pid == -1) {
        perror("fork");
        return -1;
    }

    //5.关闭内存映射区
    munmap(ptr, size);


    return 0;
}
