#include <semaphore.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>

/* a simple program to show that ipc shemaphore wait blocks the caller thread of the caller process not all the threads of the caller process */

void *worker1(void *args)
{
    while (1)
    {
        sleep(1);
        printf("worker1\n");
    }
}

void *worker2(void *args)
{
    sleep(3);
    /*sem_t sem;
    sem_init(&sem,1,0);
    printf("worker2 wait start\n");
    sem_wait(&sem);*/
    sem_t *sem =    sem_open("mysem",O_CREAT|O_RDWR,0666,0);
    sem_wait(sem);
    printf("worker2 wait end\n");
    while(1)
    {
        usleep(.8);
        printf("worker2\n");
    }
}

void main()
{
    pthread_t t1,t2;
    
    printf("starting t1\n");
    pthread_create(&t1,NULL,worker1,NULL);
    
    sleep(4);
    
    printf("starting t2\n");
    pthread_create(&t2,NULL,worker2,NULL);
    
    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
}