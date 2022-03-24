#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <unistd.h>
#include <semaphore.h>

#include "library.h"

void main()
{
    int shmid;
    size_t write_len = 0;

    int fd = open("big_download.jpg",O_CREAT|O_WRONLY);
    if (fd < 0)
    {
        perror("open");
        exit(1);
    }

    if ((shmid = shmget(KEY,SEGSIZE,0666|IPC_CREAT)) < 0)
    {
        perror("shmget");
        exit(1);
    }

    printf("shmid: %d\n",shmid);

    message_t *msg = shmat(shmid,NULL,0);
    if (msg == (void *) -1)
    {
        perror("shmat");
        exit(1);
    }

    sem_t *semread = &(msg->read);
    sem_t *semwrite = &(msg->write);

    init_semaphore(semread);
    init_semaphore(semwrite);
    
    do
    {
        sem_wait(semread);

        printf("buffsize: %lu\n", msg->buffsize);
        write_len += write(fd,&(msg->buff),msg->buffsize);

        sem_post(semwrite);
    }
    while(write_len < msg->total_size);

    shmdt(msg);

    shmctl(shmid,IPC_RMID,0);

    printf("-------- [Message] --------\n");
    printf("length: %lu\n",write_len);
    printf("----------------------------\n");

    close(fd);
}