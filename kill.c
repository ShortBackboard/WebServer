/*
    date:2023-7-25


    //1.给某个进程或者进程组发送某个信号
    int kill(pid_t pid, int sig);
    
    sig可以通过kill -l查看

    参数：
        pid:
            If pid is positive, then signal sig is sent to the process with the ID specified by pid.

            If pid equals 0, then sig is sent to every process in the process group of the calling process.

            If  pid  equals -1, then sig is sent to every process for which the calling process has permission to
            send signals, except for process 1 (init), but see below.

            If pid is less than -1, then sig is sent to every process in the process group whose ID is -pid.

RETURN VALUE
       On success (at least one signal was sent), zero is returned.  On error, -1 is returned, and errno  is
       set appropriately.


       eg:给自己发信息：kill(getpid(),xx);





    //2.给当前进程发送信号
    int raise(int sig);
    RETURN VALUE
       raise() returns 0 on success, and nonzero for failure.



    //3.发送SIGABRT信号给当前的进程，杀死当前的进程
    void abort(void);


*/

#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    //1.kill
    int ret = kill(getpid(), SIGINT);

    //2.raise
    // int ret = raise(SIGINT);

    //3.abort
    // abort();

    return 0;
}