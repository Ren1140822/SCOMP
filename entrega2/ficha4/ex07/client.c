/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * client.c
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <semaphore.h>

// Setting constants
const char *NAME_QUEUE = "sem_queue";
const char *NAME_REQUEST = "sem_request_ticket";
const char *NAME_COLLECT = "sem_collect_ticket";
const char *NAME_SHM = "sem_shm";
const char *SHM_NAME = "shm_ticket";
const int MIN_WAIT = 1;
const int MAX_WAIT = 10;

/*
 * PL 4 - Exercise 07 - CLIENT
 */
int main(int argc, char *argv[])
{
	sem_t *sem_queue, *sem_request, *sem_collect, *sem_shm;
	
	// Open queue semaphore (mutual exclusion)
	sem_queue = sem_open(NAME_QUEUE, O_CREAT, S_IRUSR|S_IWUSR, 1);
	if (sem_queue == SEM_FAILED)
	{
		perror("Semaphore failed.\n");
		exit(EXIT_FAILURE);
	}
	// Open semaphore to request a ticket (by occurence of events)
	sem_request = sem_open(NAME_REQUEST, O_CREAT, S_IRUSR|S_IWUSR, 0);
	if (sem_request == SEM_FAILED)
	{
		perror("Semaphore failed.\n");
		exit(EXIT_FAILURE);
	}
	// Open semaphore to collect a ticket (by occurence of events)
	sem_collect = sem_open(NAME_COLLECT, O_CREAT, S_IRUSR|S_IWUSR, 0);
	if (sem_collect == SEM_FAILED)
	{
		perror("Semaphore failed.\n");
		exit(EXIT_FAILURE);
	}
	// Open semaphore to access SHM (mutual exclusion)
	sem_shm = sem_open(NAME_SHM, O_CREAT, S_IRUSR|S_IWUSR, 1);
	if (sem_queue == SEM_FAILED)
	{
		perror("Semaphore failed.\n");
		exit(EXIT_FAILURE);
	}
	// Shared memory
	int fd, *ticket_no, data_size = sizeof(ticket_no);
	// Open shm
	fd = shm_open(SHM_NAME, O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
	if (fd < 0)
	{
		perror("Open shared memory failed.");
		exit(EXIT_FAILURE);
	}
	// Truncate shm
	if (ftruncate(fd, data_size) < 0)
	{
		perror("Truncating shared memory failed.");
		exit(EXIT_FAILURE);
	}
	// Map shm
	ticket_no = (int *) mmap(NULL, data_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if (ticket_no == MAP_FAILED)
	{
		perror("Mapping shared memory failed.");
		exit(EXIT_FAILURE);
	}
	
	/* Intializes random number generator */
	srand((unsigned) getpid());
	unsigned int wait_time = (rand() % MAX_WAIT) + MIN_WAIT;
	
	// Purchase ticket
	sem_wait(sem_queue); // Wait in queue
	
	sem_post(sem_request); // Request ticket
	
	sem_wait(sem_collect); // Collect ticket
	
	sem_wait(sem_shm); // Wait for exclusive access to shm
	
	sleep(wait_time);
	
	printf("My ticket number: %d (time waited: %d)\n", *ticket_no, wait_time); // Print ticket
	
	sem_post(sem_shm); // Unlock shm
	sem_post(sem_queue); // Free queue for next in line
	
	return 0;
}
