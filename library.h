#ifndef LIBRARAY_H
#define LIBRARAY_H

#include <stdio.h>
#include <semaphore.h>

#define KEY 1024
#define SEGSIZE 4096

#define MAX_BUFF_SIZE 256

typedef struct
{
    sem_t read;
    sem_t write;

    size_t total_size;
    size_t buffsize;

    char *buff;
} message_t;

#define MESSAGE_T_SIZE sizeof(message_t)

int init_semaphore(sem_t *sem);

int destroy_semaphore(sem_t *sem);

#endif