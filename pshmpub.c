#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>

#define STORAGE_ID "/SHM_TEST"
#define SEMAPHORE_ID "/SEM_TEST"
#define STORAGE_SIZE 32
#define DATA "Hello, World! From PID %d"

int main(int argc, char *argv[])
{
	int res;
	int fd;
	int len;
	pid_t pid;
	void *addr;
    sem_t *sem;
	char data[STORAGE_SIZE];

	pid = getpid();
	sprintf(data, DATA, pid);

	// get shared memory file descriptor (NOT a file)
	fd = shm_open(STORAGE_ID, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	if (fd == -1)
	{
		perror("open");
		return 10;
	}

	// extend shared memory object as by default it's initialized with size 0
	res = ftruncate(fd, STORAGE_SIZE);
	if (res == -1)
	{
		perror("ftruncate");
		return 20;
	}

	// map shared memory to process address space
	addr = mmap(NULL, STORAGE_SIZE, PROT_WRITE, MAP_SHARED, fd, 0);
	if (addr == MAP_FAILED)
	{
		perror("mmap");
		return 30;
	}

	// place data into memory
	len = strlen(data) + 1;
	memcpy(addr, data, len);

	sem = sem_open(SEMAPHORE_ID,O_CREAT|O_RDWR);
    if (NULL == sem)
    {
        perror("sem_open");
        return 40;
    }

    sem_post(sem);

	// mmap cleanup
	munmap(addr, STORAGE_SIZE);

	return 0;
}
