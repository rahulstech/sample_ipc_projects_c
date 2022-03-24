
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
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
    char   *buf;   /* Data being transferred */
};

typedef struct shmbuf shmbuf_t;

int main(int argc, char *argv[])
{
    char *shmpath = "/shm_test";
    size_t segsize = sizeof(shmbuf_t)+BUF_SIZE;

    /* Create shared memory object and set its size to the size
        of our structure. */

    int fd = shm_open(shmpath, O_CREAT|O_RDWR,0666);
    if (fd == -1)
        errExit("shm_open");

    if (ftruncate(fd,segsize) < 0)
    {
        shm_unlink(shmpath);
        perror("ftruncate");
        return 1;
    }

    /* Map the object into the caller's address space. */
    shmbuf_t *shmp = mmap(NULL, segsize,
                                PROT_READ|PROT_WRITE,
                                MAP_SHARED, fd, 0);
    if (shmp == MAP_FAILED)
        errExit("mmap");

    /* Convert data in shared memory into upper case. */

    pthread_mutex_t *lock = &(shmp->lock);
    pthread_cond_t *condread = &(shmp->condread);

    /* Initialize mutex with process shared attr */
    pthread_mutexattr_t mutexattr;
    pthread_mutexattr_init(&mutexattr);
    pthread_mutexattr_setpshared(&mutexattr,PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(lock,&mutexattr);

    /* Initialize conditions with process shared attr */
    pthread_condattr_t condattr;
    pthread_condattr_init(&condattr);
    pthread_condattr_setpshared(&condattr,PTHREAD_PROCESS_SHARED);
    pthread_cond_init(condread,&condattr);

    pthread_mutex_lock(lock);
    pthread_cond_wait(condread,lock);

    write(STDOUT_FILENO, "data received: ", 15);
    write(STDOUT_FILENO, &shmp->buf, shmp->cnt);
    write(STDOUT_FILENO, "\n", 1);

    pthread_mutex_unlock(lock);

    /* Unlink the shared memory object. Even if the peer process
        is still using the object, this is okay. The object will
        be removed only after all open references are closed. */

    pthread_mutex_destroy(lock);
    pthread_cond_destroy(condread);

    shm_unlink(shmpath);

    exit(EXIT_SUCCESS);
}