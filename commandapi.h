#ifndef COMMAND_API_H
#define COMMAND_API_H

#include <stdio.h>
#include <semaphore.h>

typedef struct
{
    sem_t semread;
    sem_t semwrite;
    ssize_t total_size; 
    size_t buff_size;
    char *buff;
} response_t;

#define SEG_SIZE sizeof(response_t)

#define SHM_PATH "/shm_4_ipc"

#endif // COMMAD_API_H