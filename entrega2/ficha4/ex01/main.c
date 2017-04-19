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
const int NUM_CHILDS = 5;
const char *SEM_NAME = "sem_mutex";
const int MUTEX = 1;
const char *FILENAME = "ex01.txt";
const int SIZE = 200;

/*
 * PL 4 - Exercise 01
 */
int main(int argc, char *argv[])
{
	// Create mutual exclusion semaphore
	sem_t *mutex;
	mutex = sem_open(SEM_NAME, O_CREAT | O_EXCL, S_IRUSR|S_IWUSR, MUTEX);
	if (mutex == SEM_FAILED)
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
	// Child processes
	if (seq > 0)
	{
		// Decrement SEM
		sem_wait(mutex);
		// Open File with write (& truncate) privileges 
		FILE *fptr = fopen(FILENAME, "a");
		// Generate numbers 
		int i;
		for (i = (seq - 1) * SIZE; i < seq * SIZE; i++)
		{
			fprintf(fptr, "%d\t", i);
		}
		fprintf(fptr, "\n");
		// Close file
		fclose(fptr);
		// Increment SEM
		sem_post(mutex);
		
		exit(EXIT_SUCCESS);
	}
	
	// Parent process
	// wait for children
	while(wait(NULL) > 0);
	// Open File with read privileges 
	FILE *fptr = fopen(FILENAME, "r");
	// Read file
	int c;
	while ( (c = fgetc(fptr)) != EOF )
	{
		fputc(c, stdout);
	}
	// Close file
	fclose(fptr);
	
	// Unlink Semaphore
	if (sem_unlink(SEM_NAME) < 0)
	{
		perror("SEM unlink failed.\n");
		exit(EXIT_FAILURE);
	}
	printf("\nSEM Unlinked with success.\n");
	// Remove file
	if(!remove(FILENAME)) 
	{
	  printf("File deleted with success.\n");
	}
	
	return 0;
}
