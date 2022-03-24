#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

typedef struct 
{
    size_t total_len;
    size_t len;
    char *message;
} msg_t;

#define SHM_PATH "/shm_test"
#define SEMREAD_PATH "/semread_test"
#define SEMWRITE_PATH "/semwrite_test"

#define SEG_SIZE 43

#define MESSAGE_FORMAT "hello from server %d\n"

int shmfd = -1;
msg_t *buff = NULL;
sem_t *semread = NULL;
sem_t *semwrite = NULL;

void destroy_shm()
{
    if (NULL != buff && buff != MAP_FAILED)
    {
        munmap(buff,SEG_SIZE);
    }
}

static void _sig_handler(int signo){
  if (signo == SIGTERM || signo == SIGINT){
    destroy_shm();
    exit(signo);
  }
}

int main(int argc, char *argv[])
{
    if (signal(SIGTERM, _sig_handler) == SIG_ERR) {
        fprintf(stderr,"Can't catch SIGTERM...exiting.\n");
        return 1;
    }

    if (signal(SIGINT, _sig_handler) == SIG_ERR) {
        fprintf(stderr,"Can't catch SIGINT...exiting.\n");
        return 1;
    }

    if ((shmfd = shm_open(SHM_PATH,O_CREAT|O_RDWR,S_IRUSR|S_IWUSR)) < 0)
    {
        perror("shm_open");
        return 1;
    }

    if (ftruncate(shmfd,SEG_SIZE) < 0)
    {
        destroy_shm();
        perror("fturncate");
        return 1;
    }

    if ((buff = mmap(NULL,SEG_SIZE,PROT_READ|PROT_WRITE,MAP_SHARED,shmfd,0)) == MAP_FAILED)
    {
        destroy_shm();
        perror("mmap");
        return 1;
    }

    if ((semread = sem_open(SEMREAD_PATH,O_RDWR)) == (void *) -1)
    {
        destroy_shm();
        perror("sem_open read");
        return 1;
    }

    if ((semwrite = sem_open(SEMWRITE_PATH,O_RDWR)) == (void *) -1)
    {
        destroy_shm();
        perror("sem_open write");
        return 1;
    }
    
    buff->total_len = 112;

    sem_post(semread);

    char message[30];
    size_t write_len = 0;
    int count = 0;

    while (write_len < buff->total_len)
    {
        count++;
        sprintf(message,MESSAGE_FORMAT,count);
        buff->len = strlen(message)+1;
        memcpy(&(buff->message),message,buff->len);
        write_len += buff->len;

        sem_post(semread);

        sem_wait(semwrite);
    }

    destroy_shm();

    return 0;
}
