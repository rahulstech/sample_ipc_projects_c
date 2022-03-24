#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <semaphore.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

void main()
{
    char name[128];
    int i, created = 0;

    for (i = 0;; i++, created++)
    {
        sprintf(name,"/multiple_sem_test_%d",i);
        if (SEM_FAILED == sem_open(name,O_CREAT|O_RDWR,0666,0))
        {
            perror("sem_open");
            break;
        }
    }

    printf("total named semaphore created: %d\n", created);

    sleep(3);
    
    for (i = 0;i < created; i++)
    {
        sprintf(name,"/multiple_sem_test_%d",i);
        sem_unlink(name);
    }
}