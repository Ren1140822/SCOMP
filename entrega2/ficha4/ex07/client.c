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

#include "shm_type.h"

// Setting constants
const char *NAME_QUEUE = "sem_queue";
const char *NAME_REQUEST = "sem_request_ticket";
const char *NAME_COLLECT = "sem_collect_ticket";
const char *NAME_SHM = "sem_shm";
const char *NAME_BARRIER1 = "sem_barrier1";
const char *NAME_BARRIER2 = "sem_barrier2";
const int MIN_WAIT = 1;
const int MAX_WAIT = 10;

/*
 * PL 4 - Exercise 07 - CLIENT
 */
int main(int argc, char *argv[])
{
	sem_t *sem_request, *sem_collect, *sem_shm, *sem_barrier1, *sem_barrier2;

	// Open semaphore to request a ticket (by occurence of events)
	sem_request = sem_open(NAME_REQUEST, O_EXCL, S_IRUSR|S_IWUSR, 0);
	if (sem_request == SEM_FAILED)
	{
		perror("Semaphore failed.\n");
		exit(EXIT_FAILURE);
	}
	// Open semaphore to collect a ticket (by occurence of events)
	sem_collect = sem_open(NAME_COLLECT, O_EXCL, S_IRUSR|S_IWUSR, 0);
	if (sem_collect == SEM_FAILED)
	{
		perror("Semaphore failed.\n");
		exit(EXIT_FAILURE);
	}
	// Open semaphore to access SHM (mutual exclusion)
	sem_shm = sem_open(NAME_SHM, O_EXCL, S_IRUSR|S_IWUSR, 1);
	if (sem_shm == SEM_FAILED)
	{
		perror("Semaphore failed.\n");
		exit(EXIT_FAILURE);
	}
	// Create a semaphore as a barrier so that each client can check if 
	// he is next in line (mutual exclusion) 
	sem_barrier1 = sem_open(NAME_BARRIER1, O_EXCL, S_IRUSR|S_IWUSR, 1);
	if (sem_barrier1 == SEM_FAILED)
	{
		perror("Semaphore failed.\n");
		exit(EXIT_FAILURE);
	}
	// Create a semaphore as a barrier so that only when all processes that 
	// have a waiting ticket pass the first barrier. This is to avoid the
	// possibility of one process controlling the barrier and entering a deadlock (by occurence of events)
	sem_barrier2 = sem_open(NAME_BARRIER2, O_EXCL, S_IRUSR|S_IWUSR, 0);
	if (sem_barrier2 == SEM_FAILED)
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
	sem_wait(sem_shm); // Wait for exclusive access to shm
	waiting_ticket = shm->waiting_ticket;
	shm->waiting_ticket++; // next waiting ticket
	sem_post(sem_shm); // Unlock shm
	
	// Waiting mechanism
	unsigned int be_served = 0, next_ticket;
	while (!be_served)
	{
		printf("I'm at b1\n");
		sem_wait(sem_barrier1); sem_post(sem_barrier1); // first barrier to block clients in queue
		
		sem_wait(sem_shm); // Wait for exclusive access to shm
		next_ticket = shm->next; // Next waiting ticket
		sem_post(sem_shm); // Unlock shm
		
		if (waiting_ticket == next_ticket) // Check if it's his turn
		{
			be_served = 1;
		}
		printf("next: %d\n", next_ticket);
		// Increment queue size
		sem_wait(sem_shm); // Wait for exclusive access to shm
		(shm->queue_size)++;
		sem_post(sem_shm); // Unlock shm
		
		// Wait for all clients that have waiting tickets
		// to pass first barrier
		if (shm->queue_size == ((shm->waiting_ticket - shm->next)))
		{
			printf("locking b1.\n");
			sem_wait(sem_barrier1); // Close first barrier
			printf("unlocking b1.\n");
			sem_post(sem_barrier2);
		}
		printf("I'm at b2\n");
		sem_wait(sem_barrier2); sem_post(sem_barrier2); // second barrier to wait for all waiting clients before proceeding
		
		// Decrement queue size
		sem_wait(sem_shm); // Wait for exclusive access to shm
		(shm->queue_size)--;
		sem_post(sem_shm); // Unlock shm
		
		// Wait for all clients that were in between barriers
		// to pass second barrier
		if (shm->queue_size == 0)
		{
			sem_wait(sem_barrier2); // Close second barrier
		}
	}
	printf("My turn\n");
	// Purchase ticket
	sem_post(sem_request); // Request ticket
	
	sem_wait(sem_collect); // Collect ticket
	
	sem_wait(sem_shm); // Wait for exclusive access to shm
	sleep(time_at_balcony); // Time in the balcony
	printf("My ticket number: %d (time at balcony: %d | waiting ticket #%d)\n", shm->ticket, time_at_balcony, waiting_ticket); // Print ticket
	
	(shm->next)++; // Next waiting ticket
	sem_post(sem_shm); // Unlock shm
	
	sem_post(sem_barrier1); // Free queue for next in line
	
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
