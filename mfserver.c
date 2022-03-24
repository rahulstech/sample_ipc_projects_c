#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <pthread.h>

#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
                        } while (0)

#define BUF_SIZE 1024   /* Maximum size for exchanged string */

/* Define a structure that will be imposed on the shared
    memory object */

struct shmbuf {
    pthread_mutex_t lock;
    pthread_cond_t condread;
    size_t cnt;             /* Number of bytes used in 'buf' */
    char   buf[BUF_SIZE];   /* Data being transferred */
};

typedef struct shmbuf shmbuf_t;

int
main(int argc, char *argv[])
{
    char *shmpath = "/shm_test";
    size_t segsize = sizeof(shmbuf_t)+BUF_SIZE;
    char *string = "this is a test line in the test.txt file\n";
    size_t len = strlen(string);

    if (len > BUF_SIZE) {
        fprintf(stderr, "String is too long\n");
        exit(EXIT_FAILURE);
    }

    /* Open the existing shared memory object and map it
        into the caller's address space. */

    int fd = shm_open(shmpath, O_CREAT|O_RDWR, 0666);
    if (fd == -1)
        errExit("shm_open");

    

    shmbuf_t *shmp = mmap(NULL, segsize,
                                PROT_READ|PROT_WRITE,
                                MAP_SHARED, fd, 0);
    if (shmp == MAP_FAILED)
        errExit("mmap");

    
    pthread_mutex_t *lock = &(shmp->lock);
    pthread_cond_t *condread = &(shmp->condread);

    /* Copy data into the shared memory object. */

    shmp->cnt = len;
    memcpy(&(shmp->buf), string, len);

    sleep(3);

    pthread_mutex_lock(lock);
    pthread_cond_signal(condread);
    pthread_mutex_unlock(lock);

    /* Write modified data in shared memory to standard output. */

    write(STDOUT_FILENO,"data sent: ",11);
    write(STDOUT_FILENO, &shmp->buf, len);
    write(STDOUT_FILENO, "\n", 1);

    exit(EXIT_SUCCESS);
}