#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <unistd.h>

void main()
{
    key_t key = 1;
    int shmid;
    size_t segsize = 64;

    if ((shmid = shmget(key,segsize,0666|IPC_CREAT)) == -1)
    {
        perror("shmget");
        exit(1);
    }

    int *data = shmat(shmid,NULL,0);
    if (data == (void *) -1)
    {
        perror("shmat");
        exit(1);
    }

    sleep(2);
    printf("Data Found: %d\n",*data);

    shmdt(data);
    shmctl(shmid,IPC_RMID,0);
}