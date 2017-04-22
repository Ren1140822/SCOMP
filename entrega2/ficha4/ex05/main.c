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
const int NUM_CHILDS = 1;
const char *SEM_NAME = "sem";

/*
 * PL 4 - Exercise 05
 */
int main(int argc, char *argv[])
{
	// Create mutual exclusion semaphore
	sem_t *sem;
	sem = sem_open(SEM_NAME, O_CREAT | O_EXCL, S_IRUSR|S_IWUSR, 0); // Sync between processes (by occurrence of events)
	if (sem == SEM_FAILED)
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
	if (seq > 0) // Child process
	{
		// Decrement SEM
		sem_wait(sem);
		
		// Print
		printf("Eu sou o filho.\n");
		
		exit(EXIT_SUCCESS);
	}
	// Parent process
	// Print
	printf("Eu sou o pai.\n");
	
	// Increment SEM
	sem_post(sem);
	
	// wait for child
	wait(NULL);
	
	// Close Semaphore
	if (sem_close(sem) < 0)
	{
		perror("SEM close failed.\n");
		exit(EXIT_FAILURE);
	}
	printf("\nSEM closed with success.\n");
	// Unlink Semaphore
	if (sem_unlink(SEM_NAME) < 0)
	{
		perror("SEM unlink failed.\n");
		exit(EXIT_FAILURE);
	}
	printf("SEM unlinked with success.\n");
	
	return 0;
}
