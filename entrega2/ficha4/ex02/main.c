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
#include "sem_array.h"

// Setting constants
const int NUM_CHILDS = 5;
const char *FILENAME = "ex02.txt";
const int SIZE = 10;

/*
 * PL 4 - Exercise 02
 */
int main(int argc, char *argv[])
{
	// Create semaphores
	sem_t *sems[NUM_CHILDS - 1];
	create_sem_array(sems, (NUM_CHILDS - 1), 0);

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
		// Related semaphore
		unsigned int sem_no = seq - 2;
		
		if (seq > 1) // Wait for previous process to complete
		{
			// Decrement SEM
			sem_wait(sems[sem_no]);
		}
		
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
		
		if (seq < NUM_CHILDS) // Unlock next semaphore
		{
			// Increment SEM
			sem_post(sems[sem_no + 1]);
		}
		
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
	unlink_sem_array(sems, (NUM_CHILDS - 1));
	printf("\nSEM Unlinked with success.\n");
	// Remove file
	if(!remove(FILENAME)) 
	{
	  printf("File deleted with success.\n");
	}
	
	return 0;
}
