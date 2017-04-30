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
#include <string.h>

#include "sem_array.h"
#include "circular_buffer.h"
#include "create_childs.h"

// Setting constants
#define SEMS_SIZE 3
#define NUM_QUEUES 4

const int MUTEX = 0;
const int FULL = 1;
const int EMPTY = 2;
const char *GENERAL_FULL_NAME = "sem_g_full";

const int NUM_PRODUCERS = 5;
const int NUM_CONSUMERS = 3;
const int LOOPS = 10;

const int P_MAX = 3;
const int P_MIN = 0;
const int S_MAX = 2;
const int S_MIN = 0;

const int TIMEOUT = 10; // Max waiting for consumer

/*
 * PL 4 - Exercise 13
 */
int main(int argc, char *argv[])
{
	int i;
	
	// Open semaphores
	sem_t *sems_m[NUM_QUEUES][SEMS_SIZE], *sem_general_full;
	int sem_values[SEMS_SIZE] = { 1 /* MUTEX */, 0 /* FULL */, CB_SIZE /* EMPTY */ };
	// Create semaphore matrix
	int name_multiplier = 0;
	for (i = 0; i < NUM_QUEUES; i++)
	{
		if (create_sem_array(sems_m[i], SEMS_SIZE, O_CREAT|O_EXCL, sem_values, name_multiplier) == NULL)
		{
			perror("Semaphore failed.\n");
			exit(EXIT_FAILURE);
		}
		name_multiplier += SEMS_SIZE;
	}
	// Semaphore to control when something is produced in any queue
	sem_general_full = sem_open(GENERAL_FULL_NAME, O_CREAT|O_EXCL, S_IRUSR|S_IWUSR, 0);
	if (sem_general_full == SEM_FAILED)
	{
		perror("Semaphore failed.\n");
		exit(EXIT_FAILURE);
	}

	// Shared memory
	circular_buffer *queues; 
	int fd, data_size = sizeof(queues) * NUM_QUEUES;
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
	queues = (circular_buffer *) mmap(NULL, data_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if (queues == MAP_FAILED)
	{
		perror("Mapping shared memory failed.");
		exit(EXIT_FAILURE);
	}
	// Init Queues
	for (i = 0; i < NUM_QUEUES; i++)
	{
		cb_init(&queues[i]);
	}

	// Create Processes (Producers)
	int seq = create_childs( (NUM_PRODUCERS + NUM_CONSUMERS) );
	if (seq < 0)
	{
		perror("fork failed.");
		exit(EXIT_FAILURE);
	}

	/***********************************************/
	if (seq > 0 && seq <= NUM_PRODUCERS) // Producers
	{
		/* Intializes random seed */
		srand((unsigned) getpid());

		int magnitude = (LOOPS / NUM_PRODUCERS);
		
		int counter = magnitude * (seq - 1);
		do
		{
			int priority = (rand() % P_MAX) + P_MIN;
			char msg[MSG_SIZE];
			snprintf(msg, MSG_SIZE, "<Producer#%d> : [Priority #%d] Message #%d produced by %d", 
			seq, priority, counter, getpid()); // Produce
			
			sem_wait(sems_m[priority][EMPTY]);
			sem_wait(sems_m[priority][MUTEX]);
			/********* CRITICAL SECTION **********/
			
			write_to_cb(&queues[priority], msg);
			//sleep((rand() % S_MAX) + S_MIN); // Take your time :)
			
			/********* CRITICAL SECTION **********/
			sem_post(sems_m[priority][MUTEX]);
			
			// Print to stdout
			puts(msg); 
			fflush(stdout);
			
			sem_post(sems_m[priority][FULL]);
			sem_post(sem_general_full);
			
			counter++;

		} while (counter < (magnitude * seq) );
		
		exit(EXIT_SUCCESS);
		
	} else 	if (seq > NUM_PRODUCERS) // Consumers
	{
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
			if (sem_timedwait(sem_general_full, &ts) == -1) // Trys to lock (decrement) before TIMEOUT
			{
				if (errno == ETIMEDOUT)
				{
					printf("timed out.\n");
					exit(EXIT_SUCCESS);
				} else
				{
					perror("sem_timedwait failed.\n");
					exit(EXIT_FAILURE);
				}
			}
			
			for (i = 0; i < NUM_QUEUES; i++)
			{
				if(sem_trywait(sems_m[i][FULL]) == 0)
				{
					char msg[MSG_SIZE];
					sem_wait(sems_m[i][MUTEX]);
					/********* CRITICAL SECTION **********/
					
					read_from_cb(&queues[i], msg);
					sleep((rand() % S_MAX) + S_MIN); // Reading takes time :)
					
					/********* CRITICAL SECTION **********/
					sem_post(sems_m[i][MUTEX]);
					
					printf("[Consumer #%d] : ( %s )\n", (seq - NUM_PRODUCERS), msg); // Consume
					fflush(stdout);
					
					sem_post(sems_m[i][EMPTY]);
					
					break; // Skip & start again
				}
			}
			
		} while (1);
	}
	
	/***********************************************/
	
	while (wait(NULL) > 0); // Wait for all child processes

	name_multiplier = 0;
	for (i = 0; i < NUM_QUEUES; i++)
	{
		// Close Semaphores
		if (close_sem_array(sems_m[i], SEMS_SIZE) < 0)
		{
			perror("SEMs close failed.\n");
			exit(EXIT_FAILURE);
		}
		// Unlink Semaphores
		if (unlink_sem_array(sems_m[i], SEMS_SIZE, name_multiplier) < 0)
		{
			perror("SEMs unlink failed.\n");
			exit(EXIT_FAILURE);
		}
		
		name_multiplier += SEMS_SIZE;
	}
	// Close & Unlink Genral FULL semaphore
	if (sem_close(sem_general_full) < 0)
	{
		perror("SEM close failed.\n");
		exit(EXIT_FAILURE);
	}
	if (sem_unlink(GENERAL_FULL_NAME) < 0)
	{
		perror("SEM unlink failed.\n");
		exit(EXIT_FAILURE);
	}
	
	// Unmap & close SHM
	if (munmap(queues, data_size) < 0)
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
	
	return 0;
}
