#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/stat.h>
#include <semaphore.h>

#include "library.h"

void main()
{
    int shmid;
    size_t maxbuffsize, read_len, write_len;

    if ((shmid = shmget(KEY,SEGSIZE,0666|IPC_CREAT)) < 0)
    {
        perror("shmget");
        exit(1);
    }

    printf("shmid: %d\n",shmid);

    message_t *msg = shmat(shmid,NULL,0);
    if (msg == (void *) -1)
    {
        perror("shmat msg");
        exit(1);
    }

    char *filename = "big_img_file.jpg";
    int fd;
    struct stat fileinfo;

    if ((fd = open(filename,O_RDONLY)) < 0)
    {
        perror("open");
        exit(1);
    }
    if (fstat(fd, &fileinfo) < 0)
    {
        perror("fstat");
        exit(1);
    }
    
    msg->total_size = fileinfo.st_size;
    
    sem_t *semread = &(msg->read);
    sem_t *semwrite = &(msg->write);

    maxbuffsize = SEGSIZE-sizeof(message_t);

    while ((read_len = read(fd,&(msg->buff),maxbuffsize)) > 0)
    {
        printf("read_len: %lu\n", read_len);
        msg->buffsize = read_len;

        sem_post(semread);

        sem_wait(semwrite);
    }

    shmdt(msg);

    close(fd);
}