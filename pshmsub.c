#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>

#define STORAGE_ID "/SHM_TEST"
#define SEMAPHORE_ID "/SEM_TEST"
#define STORAGE_SIZE 32

int main(int argc, char *argv[])
{
	int res;
	int fd;
	char data[STORAGE_SIZE];
	pid_t pid;
    sem_t *sem;
	void *addr;

	pid = getpid();

	// get shared memory file descriptor (NOT a file)
	fd = shm_open(STORAGE_ID, O_CREAT|O_RDWR|O_TRUNC, S_IRUSR | S_IWUSR);
	if (fd == -1)
	{
		perror("open");
		return 10;
	}

	// map shared memory to process address space
	addr = mmap(NULL, STORAGE_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if (addr == MAP_FAILED)
	{
		perror("mmap");
		return 30;
	}

    sem = sem_open(SEMAPHORE_ID,O_CREAT|O_RDWR);
    if (sem == NULL)
    {
        perror("sem_open");
        return 40;
    }

    sem_wait(sem);

	// place data into memory
	memcpy(data, addr, STORAGE_SIZE);

	printf("PID %d: Read from shared memory: \"%s\"\n", pid, data);

    sem_unlink(SEMAPHORE_ID);

    munmap(addr, STORAGE_SIZE);

    // shm_open cleanup
	shm_unlink(STORAGE_ID);

	return 0;
}
