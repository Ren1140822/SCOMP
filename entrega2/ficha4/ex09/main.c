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
#include <time.h>
#include <errno.h>

#include "sem_array.h"
#include "circular_buffer.h"
#include "create_childs.h"

// Setting constants
#define SEMS_SIZE 3
const int MUTEX = 0;
const int FULL = 1;
const int EMPTY = 2;

const int NUM_PRODUCERS = 2;
const int LOOPS = 30;
const int R_MAX = 100;
const int R_MIN = 0;
const int S_MAX = 2;
const int S_MIN = 0;

const int TIMEOUT = 10; // Max waiting for consumer



/*
 * PL 4 - Exercise 09
 */
int main(int argc, char *argv[])
{
	
	// Open semaphores
	sem_t *sems[SEMS_SIZE];
	int sem_values[SEMS_SIZE] = { 1 /* MUTEX */, 0 /* FULL */, CB_SIZE /* EMPTY */ };
	if (create_sem_array(sems, SEMS_SIZE, O_CREAT|O_EXCL, sem_values) == NULL)
	{
		perror("Semaphore failed.\n");
		exit(EXIT_FAILURE);
	}

	// Shared memory
	circular_buffer *cb; 
	int fd, data_size = sizeof(cb);
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
	cb = (circular_buffer *) mmap(NULL, data_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if (cb == MAP_FAILED)
	{
		perror("Mapping shared memory failed.");
		exit(EXIT_FAILURE);
	}
	// Init Circular Buffer
	cb_init(cb);


	// Create Processes (Producers)
	int seq = create_childs(NUM_PRODUCERS);
	if (seq < 0)
	{
		perror("fork failed.");
		exit(EXIT_FAILURE);
	}

	/***********************************************/
	if (seq > 0) // Producers (Childs)
	{
		/* Intializes random seed */
		srand((unsigned) getpid());
		
		int counter = 0;
		do
		{
			int item = (rand() % R_MAX) + R_MIN;
			
			sem_wait(sems[EMPTY]);
			sem_wait(sems[MUTEX]);
			/********* CRITICAL SECTION **********/
			write_to_cb(cb, item);
			
			/********* CRITICAL SECTION **********/
			sem_post(sems[MUTEX]);
			
			printf("P#%d Produced: %d\n", seq, item); // Produce
			fflush(stdout);
			
			sem_post(sems[FULL]);
			
			counter++;

		} while (counter < (LOOPS / NUM_PRODUCERS));
		
		exit(EXIT_SUCCESS);
	}
	
	// Consumer (Parent)
	/* Intializes random seed */
	srand((unsigned) getpid());
	
	struct timespec ts;
	if (clock_gettime(CLOCK_REALTIME, &ts) == -1)
	{
		perror("close_gettime failed.\n");
		exit(EXIT_FAILURE);
	}
	ts.tv_sec += TIMEOUT;
	
	do
	{
		if (sem_timedwait(sems[FULL], &ts) == -1) // Trys to lock (decrement) before TIMEOUT
		{
			if (errno == ETIMEDOUT)
			{
				printf("timed out.\n");
				break;
			} else
			{
				perror("sem_timedwait failed.\n");
				exit(EXIT_FAILURE);
			}
		}
		
		sem_wait(sems[MUTEX]);
		/********* CRITICAL SECTION **********/
		
		int item = read_from_cb(cb);
		
		/********* CRITICAL SECTION **********/
		sem_post(sems[MUTEX]);
		
		printf("Consumed: %d\n", item); // Consume
		fflush(stdout);
		sleep((rand() % S_MAX) + S_MIN); // Simulate a delay
		
		sem_post(sems[EMPTY]);
		
	} while (1);
	
	while (wait(NULL) > 0);
	
	/***********************************************/
	
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
	if (munmap(cb, data_size) < 0)
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
	printf("Circular Buffer unlinked.\n");
	
	return 0;
}
