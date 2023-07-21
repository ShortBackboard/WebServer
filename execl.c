/*
    date:2023-7-21

    exec函数族的作用是根据指定的文件名找到可执行文件，并用它来取代调用原有进程的内容
    不会创建新的进程
    

    int execl(const char *pathname, const char *arg, ...)
        -path: 需要执行的文件路径+名称(绝对路径)

        -arg: 执行可执行文件所需要的参数,  
        第一个arg指向filename，一般无实际作用
        从第二个参数开始往后就是执行文件执行需要的参数列表
        参数最后需要以NULL结束(哨兵);
        eg: ls -l a.txt

    
    返回值：    
        执行成功不会返回；(成功，内容被覆盖)
        只有调用失败，返回-1；从原程序的调用点接着向下执行



    int execlp(const char *file, const char *arg, ...);
    按照系统环境变量中查找可执行文件



    int execv(const char *pathname, char *const argv[]);
    argv[]: 需要的参数的字符串数组
    eg: char *argv[] = {"/home/cnu/summerStudy/hello", "hello", "LCY", NULL};
    方便重用



    int execve(const char *pathname, char *const argv[],
                  char *const envp[]);
    
    -argv[]: 参数字符串数组
    -envp[]: 自定义环境变量数组

    eg : char *envp[] = {"/home/aaa","/home/bbb"};
    会按照自定义的环境变量路径中去找环境变量




    l(list): 参数地址列表，以空指针结尾
    v(vector): 存有各参数地址的指针数组的地址
    p(path): 按PATH环境变量指定的目录搜索可执行文件，系统环境变量
    e(environment): 存有环境变量字符串地址的指针数组的地址，自定义环境变量

*/

#include <unistd.h> 
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>


int main() {
    //fork一个子进程，在子进程中exec

    int pid = fork();

    if(pid == -1) {
        perror("fork");
        return -1;

    }else if(pid > 0) {
        //父进程
        printf("I am parent process, pid: %d.\n", getpid());

    }else if(pid == 0) {
        //子进程
        //hello:可执行文件的名称
        int ret = execl("/home/cnu/summerStudy/hello", "hello", "LCY", NULL);
        
        if(ret == -1){
            perror("execl");
            return -1;
        }

        //如果execl成功，下面语句不会执行(被覆盖)
        printf("I am child process, pid: %d.\n", getpid());
    }

    return 0;
}