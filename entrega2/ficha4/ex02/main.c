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
const int SIZE = 200;

/*
 * PL 4 - Exercise 02
 */
int main(int argc, char *argv[])
{
	// Create semaphores
	sem_t *sems[NUM_CHILDS];
	if (create_sem_array(sems, (NUM_CHILDS), 0) == NULL)
	{
		perror("Semaphores failed.\n");
		exit(EXIT_FAILURE);
	}
	// Set first semaphore's value to 1
	sem_post(sems[0]);

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
		// Related SEM & next SEM
		unsigned int cur_sem_no = seq - 1;
		unsigned int nxt_sem_no = seq % NUM_CHILDS;
		// Decrement SEM
		sem_wait(sems[cur_sem_no]);
		
		// Open File with write (& truncate) privileges 
		FILE *fptr = fopen(FILENAME, "a");
		// Generate numbers 
		int i;
		for (i = (seq - 1) * SIZE; i < seq * SIZE; i++)
		{
			fprintf(fptr, "%d ", i);
		}
		fprintf(fptr, "\n");
		// Close file
		fclose(fptr);

		// Increment SEM
		sem_post(sems[nxt_sem_no]);
		
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
	
	// Close semaphores
	if (close_sem_array(sems, NUM_CHILDS) < 0)
	{
		perror("SEM close failed.\n");
		exit(EXIT_FAILURE);
	}
	printf("\nSEM closed with success.\n");
	// Unlink semaphores
	if (unlink_sem_array(sems, NUM_CHILDS) < 0)
	{
		perror("SEM unlink failed.\n");
		exit(EXIT_FAILURE);
	}
	printf("SEM unlinked with success.\n");
	// Remove file
	if(!remove(FILENAME)) 
	{
	  printf("File deleted with success.\n");
	}
	
	return 0;
}
