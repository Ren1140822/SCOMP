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

#include "shm_type.h"

// Setting constants
const char *NAME_REQUEST = "sem_request_ticket";
const char *NAME_COLLECT = "sem_collect_ticket";
const char *NAME_SHM = "sem_shm";
const char *NAME_BARRIER1 = "sem_barrier1";
const char *NAME_BARRIER2 = "sem_barrier2";
const int FIRST_TICKET = 1000;
const int NUM_TICKETS = 10;


/*
 * PL 4 - Exercise 07 - SELLER
 */
int main(int argc, char *argv[])
{
	// Print something
	printf("\nThe THEATER is open!\n");
	fflush(stdout);
	
	sem_t *sem_request, *sem_collect, *sem_shm, *sem_barrier1, *sem_barrier2;

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
	if (sem_shm == SEM_FAILED)
	{
		perror("Semaphore failed.\n");
		exit(EXIT_FAILURE);
	}
	// Create a semaphore as a barrier so that each client can check if 
	// he is next in line (mutual exclusion) 
	sem_barrier1 = sem_open(NAME_BARRIER1, O_CREAT | O_EXCL, S_IRUSR|S_IWUSR, 1);
	if (sem_barrier1 == SEM_FAILED)
	{
		perror("Semaphore failed.\n");
		exit(EXIT_FAILURE);
	}
	// Create a semaphore as a barrier so that only when all processes that 
	// have a waiting ticket pass the first barrier. This is to avoid the
	// possibility of one process controlling the barrier and entering a deadlock (by occurence of events)
	sem_barrier2 = sem_open(NAME_BARRIER2, O_CREAT | O_EXCL, S_IRUSR|S_IWUSR, 0);
	if (sem_barrier2 == SEM_FAILED)
	{
		perror("Semaphore failed.\n");
		exit(EXIT_FAILURE);
	}

	// Shared memory
	shm_type *shm; 
	int fd, data_size = sizeof(shm_type);
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
	shm = (shm_type *) mmap(NULL, data_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if (shm == MAP_FAILED)
	{
		perror("Mapping shared memory failed.");
		exit(EXIT_FAILURE);
	}

	// Ticket counter
	int first_ticket = FIRST_TICKET;
	int counter = 0;
	
	do {
		sem_wait(sem_request); // Wait for ticket request
		
		sem_wait(sem_shm); // Wait for exclusive access to shm
		// Issue ticket
		shm->ticket = first_ticket + counter;
		sem_post(sem_shm); // Unlock shm
		
		counter++; // next ticket
		
		sem_post(sem_collect); // Unlock ticket collection 
		
	} while(counter <= NUM_TICKETS);
	
	// Close Semaphores
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
	if (sem_close(sem_barrier1) < 0)
	{
		perror("SEM close failed.\n");
		exit(EXIT_FAILURE);
	}
	if (sem_close(sem_barrier2) < 0)
	{
		perror("SEM close failed.\n");
		exit(EXIT_FAILURE);
	}
	// Unlink Semaphores
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
	if (sem_unlink(NAME_BARRIER1) < 0)
	{
		perror("SEM unlink failed.\n");
		exit(EXIT_FAILURE);
	}
	if (sem_unlink(NAME_BARRIER2) < 0)
	{
		perror("SEM unlink failed.\n");
		exit(EXIT_FAILURE);
	}
	printf("\nSEMs unlinked.\n");
	
	// Unmap & close SHM
	if (munmap(shm, data_size) < 0)
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
