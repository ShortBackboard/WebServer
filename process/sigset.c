/*
    date:2023-7-26

    信号集:多个信号的集合

    1.用户通过键盘  Ctrl + C, 产生2号信号SIGINT (信号被创建)

    2.信号产生但是没有被处理 （未决）
     - 在内核中将所有的没有被处理的信号存储在一个集合中 （未决信号集）
        - SIGINT信号状态被存储在第二个标志位上
            - 这个标志位的值为0， 说明信号不是未决状态
            - 这个标志位的值为1， 说明信号处于未决状态
    
    3.这个未决状态的信号，需要被处理，处理之前需要和另一个信号集（阻塞信号集），进行比较
        - 阻塞信号集默认不阻塞任何的信号
        - 如果想要阻塞某些信号需要用户调用系统的API

    4.在处理的时候和阻塞信号集中的标志位进行查询，看是不是对该信号设置阻塞了
        - 如果没有阻塞，这个信号就被处理
        - 如果阻塞了，这个信号就继续处于未决状态，直到阻塞解除，这个信号就被处理



    
    以下信号集相关的函数都是对自定义的信号集进行操作。

    sigset_t:一个64位的整数

    int sigemptyset(sigset_t *set);
        - 功能：清空信号集中的数据,将信号集中的所有的标志位置为0
        - 参数：set,传出参数，需要操作的信号集
        - 返回值：成功返回0， 失败返回-1

    int sigfillset(sigset_t *set);
        - 功能：将信号集中的所有的标志位置为1
        - 参数：set,传出参数，需要操作的信号集
        - 返回值：成功返回0， 失败返回-1

    int sigaddset(sigset_t *set, int signum);//添加
        - 功能：设置信号集中的某一个信号对应的标志位为1，表示阻塞这个信号
        - 参数：
            - set：传出参数，需要操作的信号集
            - signum：需要设置阻塞的那个信号
        - 返回值：成功返回0， 失败返回-1

    int sigdelset(sigset_t *set, int signum);//删除
        - 功能：设置信号集中的某一个信号对应的标志位为0，表示不阻塞这个信号
        - 参数：
            - set：传出参数，需要操作的信号集
            - signum：需要设置不阻塞的那个信号
        - 返回值：成功返回0， 失败返回-1

    int sigismember(const sigset_t *set, int signum);//判断
        - 功能：判断某个信号是否阻塞
        - 参数：
            - set：需要操作的信号集
            - signum：需要判断的那个信号
        - 返回值：
            1 ： signum被阻塞
            0 ： signum不阻塞
            -1 ： 失败
*/

#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    //创建一个信号集
    sigset_t set;

    //清空信号集内容
    sigemptyset(&set);

    //判断 SIGINT 是否在信号集里面
    int ret1 = sigismember(&set, SIGINT);

    if(ret1 == 0) {
        printf("1st: SIGINT is not blocked\n");
    }

    //添加信号到信号集
    sigaddset(&set, SIGINT);//2号信号

    int ret2 = sigismember(&set, SIGINT);

    if(ret2 == 1) {
        printf("2nd: SIGINT is blocked\n");
    }

    //删除一个信号
    sigdelset(&set, SIGINT);

    int ret3 = sigismember(&set, SIGINT);

    if(ret3 == 0) {
        printf("3rd: SIGINT is not blocked\n");
    }

    return 0;
} 