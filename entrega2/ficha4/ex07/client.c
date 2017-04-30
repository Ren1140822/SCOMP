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

#include "sem_array.h"
#include "shm_type.h"

// Setting constants
#define SEMS_SIZE 6

const int S_REQUEST = 0; 	// semaphore to request a ticket (by occurence of events)

const int S_COLLECT = 1; 	// semaphore to collect a ticket (by occurence of events)

const int S_SHM = 2; 		// semaphore to access SHM (mutual exclusion)

const int S_BARRIER1 = 3; 	// semaphore as a barrier so that each client can check if 
							// he is next in line (mutual exclusion)
							 
const int S_BARRIER2 = 4;	// semaphore as a barrier so that only when all processes that 
							// have a waiting ticket pass the first barrier. This is to avoid the
							// possibility of one process controlling the barrier and entering a deadlock (by occurence of events)

const int S_QUEUE = 5; 		// semaphore to control a queue

const int MIN_WAIT = 1;
const int MAX_WAIT = 10;

/*
 * PL 4 - Exercise 07 - CLIENT
 */
int main(int argc, char *argv[])
{
	// Open semaphores
	int sem_values[SEMS_SIZE] = { 0, 0, 1, 1, 0, 0 };
	sem_t *sems[SEMS_SIZE];
	if (create_sem_array(sems, SEMS_SIZE, O_EXCL, sem_values) == NULL)
	{
		perror("Semaphore failed.\n");
		exit(EXIT_FAILURE);
	}
	
	// Shared memory
	shm_type *shm; 
	int fd, data_size = sizeof(shm_type);
	// Open shm
	fd = shm_open(SHM_NAME, O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
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
	
	/* Intializes random sleep time */
	srand((unsigned) getpid());
	unsigned int time_at_balcony = (rand() % MAX_WAIT) + MIN_WAIT;
	
	// Take a waiting ticket
	int waiting_ticket;
	sem_wait(sems[S_SHM]); // Wait for exclusive access to shm
	waiting_ticket = shm->waiting_ticket;
	shm->waiting_ticket++; // next waiting ticket
	sem_post(sems[S_SHM]); // Unlock shm
	
	// Waiting mechanism
	unsigned int be_served = 0;
	while (!be_served)
	{
		sem_wait(sems[S_BARRIER1]); sem_post(sems[S_BARRIER1]); // first barrier to block clients in queue
		
		if (waiting_ticket == shm->next) // Check if it's his turn
		{
			be_served = 1;
		}
		
		// to pass first barrier and then close barrier 1 & open barrier 2
		/**************************************************************/
		sem_wait(sems[S_SHM]); // Wait for exclusive access to shm
		/***************** CRITICAL SECTION ***************************/
		sem_post(sems[S_QUEUE]); // Increment queue
		
		int num_clients; 
		sem_getvalue(sems[S_QUEUE], &num_clients); // Get number of clients in queue
		
		if ( num_clients == (shm->waiting_ticket - shm->next) ) // Check if last client is between barriers
		{
			sem_wait(sems[S_QUEUE]); // Decrement by 1 queue
			sem_wait(sems[S_BARRIER1]); // Close barrier 1
			sem_post(sems[S_BARRIER2]); // Open barrier 2
		}
		/***************** CRITICAL SECTION ***************************/
		sem_post(sems[S_SHM]); // Unlock shm
		/**************************************************************/
		
		sem_wait(sems[S_BARRIER2]); sem_post(sems[S_BARRIER2]); // second barrier block waiting clients before proceeding
		/** This barrier prevents that 1 process can enter an infinite loop by being always the 
		 * first to pass the barrier 1 and not being the next to be served**/
		
		// Decrement queue size
		if (sem_trywait(sems[S_QUEUE]) < 0) // Last client passing the barrier 2 closes barrier 2
		{
			sem_wait(sems[S_BARRIER2]); // Close barrier 2
		}
	}
	
	// Purchase ticket
	sem_post(sems[S_REQUEST]); // Request ticket
	
	sem_wait(sems[S_COLLECT]); // Collect ticket
	
	sleep(time_at_balcony); // Time at the balcony
	
	/**************************************************************/
	sem_wait(sems[S_SHM]); // Wait for exclusive access to shm
	/***************** CRITICAL SECTION ***************************/
	printf("My ticket number: %d (time at balcony: %d | waiting ticket #%d)\n", shm->ticket, time_at_balcony, waiting_ticket); // Print ticket
	(shm->next)++; // Next waiting ticket
	/***************** CRITICAL SECTION ***************************/
	sem_post(sems[S_SHM]); // Unlock shm
	/**************************************************************/
	
	sem_post(sems[S_BARRIER1]); // Free queue for next in line
	
	// Close Semaphores
	if (close_sem_array(sems, SEMS_SIZE) < 0)
	{
		perror("SEMs close failed.\n");
		exit(EXIT_FAILURE);
	}
	
	// Unmap & close SHM
	if (munmap(shm, data_size) < 0)
	{
		exit(EXIT_FAILURE);
	}
	if (close(fd) < 0)
	{
		exit(EXIT_FAILURE);
	}
	
	return 0;
}
