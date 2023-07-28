/*
    date:2023-7-28

    消息队列

    gcc messageQueue.c -o mq -lrt
    
    mqd_t mq_send(mqd_t mqdes, const char *msg_ptr, size_t msg_len, unsigned msg_prio);
    msg_prio：消息优先级 它是一个小于MQ_PRIO_MAX的数，数值越大，优先级越高。
    POSIX消息队列在调用mq_receive时总是返回队列中最高优先级的最早消息。
    如果消息不需要设定优先级，那么可以在mq_send是置msg_prio为0，mq_receive的msg_prio置为NULL。
    返回值：成功返回0；失败返回-1


    //todo:接收的消息过长
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <sys/wait.h>

#define MAX_MSG_SIZE 128  //max length
#define QUEUE_NAME "/queue" //queue's name begin with "/"

int main() {
    mqd_t mqd;//message queue descriptor
    pid_t pid;
    struct mq_attr attr;
    char buffer[MAX_MSG_SIZE];

    
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_maxmsg = 20;

    // create message queue
    //mq_open:return Message queue descriptor if create successfully, and return -1 if error.
    
    mqd = mq_open(QUEUE_NAME, O_CREAT | O_RDWR, 0664, &attr);
    if (mqd == -1) {
        perror("create");
        exit(1);
    }

    pid = fork();
    if (pid < 0) {//error
        fprintf(stderr, "Fork failed\n");
        exit(1);
    } 
    else if (pid == 0) { // child process      
        printf("(childProcess) Please enter a string (length < 128): \n");
        
        fgets(buffer, MAX_MSG_SIZE, stdin);

        //mq_send: return 0 if success and return -1 if error
        if (mq_send(mqd, buffer, strlen(buffer)+1, 0) == -1) {
            perror("send");
            exit(1);
        }
    } 
    else {// parent process   
        wait(NULL);
        if (mq_receive(mqd, buffer, MAX_MSG_SIZE, NULL) == -1) {
            perror("receive");
            exit(1);
        }
        printf("(parentProcess) Received the string: %s \n", buffer);
    }

    mq_close(mqd);
    return 0;
}

