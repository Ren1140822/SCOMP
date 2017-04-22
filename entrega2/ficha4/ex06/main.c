/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * main.c
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

#include "create_childs.h"

// Setting constants
#define NUM_SEMS 2
const int NUM_CHILDS = 1;
const char *SEM_NAME[NUM_SEMS] = {"sem_parent", "sem_child"};
const int PARENT_NO = 0;
const int CHILD_NO = 1;
const int N_TIMES = 10;

/*
 * PL 4 - Exercise 06
 */
int main(int argc, char *argv[])
{
	// Create mutual exclusion semaphore
	sem_t *sems[NUM_SEMS];
	// Sync between processes (by occurrence of events)
	sems[PARENT_NO] = sem_open(SEM_NAME[PARENT_NO], O_CREAT | O_EXCL, S_IRUSR|S_IWUSR, 1); // Semaphore parent (mutual exclusion)
	sems[CHILD_NO] = sem_open(SEM_NAME[CHILD_NO], O_CREAT | O_EXCL, S_IRUSR|S_IWUSR, 0); // Semaphore child (by occurrence of events)
	if (sems[PARENT_NO] == SEM_FAILED || sems[CHILD_NO] == SEM_FAILED)
	{
		perror("Semaphore failed.\n");
		exit(EXIT_FAILURE);
	}
	// Create new process
	int seq = create_childs(NUM_CHILDS);
	if (seq < 0)
	{
		perror("Fork failed.\n");
		exit(EXIT_FAILURE);
	}
	// Both processes
	int i;
	for (i = 0; i < N_TIMES; i++)
	{
		// Decrement SEM
		sem_wait(sems[seq]);
	
		// Print
		printf("Eu sou o %s.\n", (seq > 0) ? "filho" : "pai");
		
		// Increment SEM
		sem_post(sems[(seq + 1) % NUM_SEMS]);
	}

	if (seq == 0) // Parent
	{
		// wait for child
		wait(NULL);
		
		for (i = 0; i < NUM_SEMS; i++) // Close & unlink both SEM
		{
			// Close Semaphore
			if (sem_close(sems[i]) < 0)
			{
				perror("SEM close failed.\n");
				exit(EXIT_FAILURE);
			}
			// Unlink Semaphore
			if (sem_unlink(SEM_NAME[i]) < 0)
			{
				perror("SEM unlink failed.\n");
				exit(EXIT_FAILURE);
			}
		}
		printf("\nSEMs unlinked & closed with success.\n");
	}
	
	return 0;
}
