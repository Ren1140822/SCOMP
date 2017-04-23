/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * seller.c
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
const int OPEN_TIME = 60;
const int NUM_TICKETS = 10;

/*
 * PL 4 - Exercise 07 - SELLER
 */
int main(int argc, char *argv[])
{
	sem_t *sem_queue, *sem_request, *sem_collect, *sem_shm;
	
	// Create a queue semaphore (mutual exclusion)
	sem_queue = sem_open(NAME_QUEUE, O_CREAT | O_EXCL, S_IRUSR|S_IWUSR, 1);
	if (sem_queue == SEM_FAILED)
	{
		perror("Semaphore failed.\n");
		exit(EXIT_FAILURE);
	}
	// Create semaphore to request a ticket (by occurence of events)
	sem_request = sem_open(NAME_REQUEST, O_CREAT | O_EXCL, S_IRUSR|S_IWUSR, 0);
	if (sem_request == SEM_FAILED)
	{
		perror("Semaphore failed.\n");
		exit(EXIT_FAILURE);
	}
	// Create semaphore to collect a ticket (by occurence of events)
	sem_collect = sem_open(NAME_COLLECT, O_CREAT | O_EXCL, S_IRUSR|S_IWUSR, 0);
	if (sem_collect == SEM_FAILED)
	{
		perror("Semaphore failed.\n");
		exit(EXIT_FAILURE);
	}
	// Create a semaphore to access SHM (mutual exclusion)
	sem_shm = sem_open(NAME_SHM, O_CREAT | O_EXCL, S_IRUSR|S_IWUSR, 1);
	if (sem_queue == SEM_FAILED)
	{
		perror("Semaphore failed.\n");
		exit(EXIT_FAILURE);
	}
	
	// Shared memory
	int fd, *ticket_no, data_size = sizeof(ticket_no);
	// Open shm
	fd = shm_open(SHM_NAME, O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
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

	// Ticket counter
	int counter = 1;
	
	do {
		sem_wait(sem_request); // Wait for ticket request
		sem_wait(sem_shm); // Wait for exclusive access to shm
		
		// Issue ticket
		*ticket_no = counter;
		
		sem_post(sem_shm); // Unlock shm
		sem_post(sem_collect); // Unlock ticket collection 
		
		counter++; // next ticket
	} while(counter <= NUM_TICKETS);
	
	// Close Semaphores
	if (sem_close(sem_queue) < 0)
	{
		perror("SEM close failed.\n");
		exit(EXIT_FAILURE);
	}
	if (sem_close(sem_request) < 0)
	{
		perror("SEM close failed.\n");
		exit(EXIT_FAILURE);
	}
	if (sem_close(sem_collect) < 0)
	{
		perror("SEM close failed.\n");
		exit(EXIT_FAILURE);
	}
	if (sem_close(sem_shm) < 0)
	{
		perror("SEM close failed.\n");
		exit(EXIT_FAILURE);
	}
	// Unlink Semaphores
	if (sem_unlink(NAME_QUEUE) < 0)
	{
		perror("SEM unlink failed.\n");
		exit(EXIT_FAILURE);
	}
	if (sem_unlink(NAME_REQUEST) < 0)
	{
		perror("SEM unlink failed.\n");
		exit(EXIT_FAILURE);
	}
	if (sem_unlink(NAME_COLLECT) < 0)
	{
		perror("SEM unlink failed.\n");
		exit(EXIT_FAILURE);
	}
	if (sem_unlink(NAME_SHM) < 0)
	{
		perror("SEM unlink failed.\n");
		exit(EXIT_FAILURE);
	}
	printf("\nSEMs unlinked.\n");
	
	// Unmap & close SHM
	if (munmap(ticket_no, data_size) < 0)
	{
		exit(EXIT_FAILURE);
	}
	if (close(fd) < 0)
	{
		exit(EXIT_FAILURE);
	}
	// Unlink SHM
	if (shm_unlink(SHM_NAME) < 0)
	{
		exit(EXIT_FAILURE);
	}	
	printf("SHM Unlinked.\n");
	
	return 0;
}
