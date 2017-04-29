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

#include "sem_array.h"
#include "shm_type.h"

// Setting constants
#define SEMS_SIZE 5

const int S_REQUEST = 0; 	// semaphore to request a ticket (by occurence of events)

const int S_COLLECT = 1; 	// semaphore to collect a ticket (by occurence of events)

const int S_SHM = 2; 		// semaphore to access SHM (mutual exclusion)

const int S_BARRIER1 = 3; 	// semaphore as a barrier so that each client can check if 
							// he is next in line (mutual exclusion)
							 
const int S_BARRIER2 = 4;	// semaphore as a barrier so that only when all processes that 
							// have a waiting ticket pass the first barrier. This is to avoid the
							// possibility of one process controlling the barrier and entering a deadlock (by occurence of events)


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
	
	// Open semaphores
	sem_t *sems[SEMS_SIZE];
	int sem_values[SEMS_SIZE] = { 0, 0, 1, 1, 0 };
	if (create_sem_array(sems, SEMS_SIZE, O_CREAT|O_EXCL, sem_values) == NULL)
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
		sem_wait(sems[S_REQUEST]); // Wait for ticket request
		
		sem_wait(sems[S_SHM]); // Wait for exclusive access to shm
		// Issue ticket
		shm->ticket = first_ticket + counter;
		sem_post(sems[S_SHM]); // Unlock shm
		
		counter++; // next ticket
		
		sem_post(sems[S_COLLECT]); // Unlock ticket collection 
		
	} while(counter <= NUM_TICKETS);
	
	// Close Semaphores
	if (close_sem_array(sems, SEMS_SIZE) < 0)
	{
		perror("SEMs close failed.\n");
		exit(EXIT_FAILURE);
	}
	
	// Unlink Semaphores
	if (unlink_sem_array(sems, SEMS_SIZE) < 0)
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
