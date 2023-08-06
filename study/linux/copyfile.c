/*
    date:2023-7-17
    program:read and write function use to copy a file and append words


    //read
    ssize_t read(int fd, void *buf, size_t count);
    -fd:文件描述符，通过open获得
    -buf:需要读取的数据的存放位置，数组的地址
    -count:指定数组的大小

    返回值：
        -成功
            >0:返回实际读取到的字节数
            =0:读取到文件末尾
        
        -失败
            -1


    //write
    ssize_t write(int fd, void *buf, size_t count);
    返回值：写入文档的字节数（成功）；-1（出错）

*/

#include <unistd.h> //read
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(){
    //1.打开要复制的文件
    int srcfd = open("from.txt", O_RDONLY);
    if(srcfd == -1) {
        perror("open file error");
        return -1;
    }

    //2.创建新文件
    int destfd = open("to.txt", O_WRONLY | O_CREAT, 0664);
    if(destfd == -1) {
        perror("create new file error");
        return -1;
    }

    //3.频繁读写
    char buf[1024] = {0};//数组
    int len = 0;

    while((len = read(srcfd, buf, sizeof(buf))) > 0) {
        write(destfd, buf, len);
        printf("per read length:%d\n",len);//此例子文件内容长度小于设置的1024大小，所以就一次读写
    }

    //4.在新文件末尾追加内容
    int appendfd = open("to.txt", O_WRONLY | O_APPEND);

    if(appendfd == -1) {
        perror("append error");
        return -1;
    }

    char append[1024] = {"hello,world!\n"};
    
    int appendwrite = write(appendfd, append, sizeof(append));
    if(appendwrite == -1) {
        perror("append write error");
        return -1;
    }

    //5.关闭文件
    close(appendfd);
    close(destfd);
    close(srcfd);


    return 0;
}


