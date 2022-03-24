#include <semaphore.h>
#include "library.h"

int init_semaphore(sem_t *sem)
{
    return sem_init(sem,1,0) < 0 ? 0 : 1;
}

int destroy_semaphore(sem_t *sem)
{
    return sem_destroy(sem) < 0 ? 0 : 1;
}

/*
void init_process_shared_lock_and_conditions(message_t *msg)
{
    pthread_mutex_t *mutex = &(msg->mutex);
    pthread_mutexattr_t *mutexattr = &(msg->mutexattr);
    pthread_cond_t *cond_read = &(msg->cond_read);
    pthread_condattr_t *condattr_read = &(msg->condattr_read);
    pthread_cond_t *cond_write = &(msg->cond_write);
    pthread_condattr_t *condattr_write = &(msg->condattr_write);

    pthread_mutexattr_init(mutexattr);
    pthread_mutexattr_setpshared(mutexattr,PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(mutex,mutexattr);

    pthread_condattr_init(condattr_read);
    pthread_condattr_setpshared(condattr_read,PTHREAD_PROCESS_SHARED);
    pthread_cond_init(cond_read,condattr_read);

    pthread_condattr_init(condattr_write);
    pthread_condattr_setpshared(condattr_write,PTHREAD_PROCESS_SHARED);
    pthread_cond_init(cond_write,condattr_write);
}

void destroy_process_shared_lock_and_condition(message_t *msg)
{
    pthread_mutex_t *mutex = &(msg->mutex);
    pthread_mutexattr_t *mutexattr = &(msg->mutexattr);
    pthread_cond_t *cond_read = &(msg->cond_read);
    pthread_condattr_t *condattr_read = &(msg->condattr_read);
    pthread_cond_t *cond_write = &(msg->cond_write);
    pthread_condattr_t *condattr_write = &(msg->condattr_write);

    pthread_mutex_destroy(mutex);
    pthread_mutexattr_destroy(mutexattr);
    pthread_cond_destroy(cond_read);
    pthread_cond_destroy(cond_write);
    pthread_condattr_destroy(condattr_read);
    pthread_condattr_destroy(condattr_write);
}
*/