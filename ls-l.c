/*
date:2023-7-18
A program that imitate ls -l command.
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h> //strncpy

#include <pwd.h>//通过uid获得用户名称
#include <grp.h>//通过gid获得用户组名称
#include <time.h>//将时间秒数转化为具体的时间ctime



int main(int argc, char *argv[]){
    //argc: 可执行文件参数数量  eg：./app 1.c   argc = 2
    //argv[]: 参数内容          eg: ./app 1.c  argv[0] = ./app; argv[1] = 1.c

    //判断输入的参数是否正确
    if(argc < 2) {
        printf("%s filename\n", argv[0]);
        return -1;
    }

    //通过stat函数获取文件的信息
    struct stat st;
    int ret = stat(argv[1], &st);
    if(ret == -1) {
        perror("stat");
        return -1;
    }

    //获取文件类型和文件权限
    char perms[11] = {0}; //用于保存文件类型和文件权限

    switch(st.st_mode & __S_IFMT) {
        case __S_IFLNK:
            perms[0] = 'l';
            break;
        
        case __S_IFDIR:
            perms[0] = 'd';
            break;

        case __S_IFREG:
            perms[0] = '-';
            break;
        
        case __S_IFBLK:
            perms[0] = 'b';
            break;

        case __S_IFCHR:
            perms[0] = 'c';
            break;
        
        case __S_IFSOCK:
            perms[0] = 's';
            break;

        case __S_IFIFO:
            perms[0] = 'p';
            break;
        
        default:
            perms[0] = '?';
            break;
    }

    // 文件所有者
    perms[1] = (st.st_mode & S_IRUSR) ? 'r' : '-';
    perms[2] = (st.st_mode & S_IWUSR) ? 'w' : '-';
    perms[3] = (st.st_mode & S_IXUSR) ? 'x' : '-';

    // 文件所在组
    perms[4] = (st.st_mode & S_IRGRP) ? 'r' : '-';
    perms[5] = (st.st_mode & S_IWGRP) ? 'w' : '-';
    perms[6] = (st.st_mode & S_IXGRP) ? 'x' : '-';

    // 其他人
    perms[7] = (st.st_mode & S_IROTH) ? 'r' : '-';
    perms[8] = (st.st_mode & S_IWOTH) ? 'w' : '-';
    perms[9] = (st.st_mode & S_IXOTH) ? 'x' : '-';

    // 硬连接数
    int linkNum = st.st_nlink;

    //文件所有者    通过uid获得用户名称
    char *fileUser = getpwuid(st.st_uid)->pw_name;

    //文件所在组    通过gid获得用户组名称
    char *fileGrp = getgrgid(st.st_gid)->gr_name;

    //文件大小
    long int fileSize = st.st_size;

    //获取文件修改的时间
    char *time = ctime(&st.st_mtime); //从1970年1月1日0点到目前的秒数
    
    char mtime[512] = {0};
    strncpy(mtime, time, strlen(time) - 1); //去掉换行符

    //保存所有文件信息
    char buf[1024];

    //sprintf:主要功能是把格式化的数据写入某个字符串中
    sprintf(buf,"%s %d %s %s %ld %s %s", perms, linkNum, fileUser, fileGrp, fileSize, mtime, argv[1]);

    printf("%s\n", buf);

    return 0;
}