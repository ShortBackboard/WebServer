#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>


int main(int argc, char *argv[]) {
    if(argc == 2){
        printf("(For execl) hello, %s!\n", argv[1]);
    }else {
        printf("%s name.\n", argv[0]);
    }

    printf("in execHello.c, ppid :%d.\n", getppid());

    return 0;
}