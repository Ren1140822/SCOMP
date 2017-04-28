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
#include <string.h>

// Setting constants
const char *SEM_NAME = "sem_mutex";
const char *DEFAULT = "input.txt";
const char *UNLINK_CMD = "-u";
const int FILENAME_SIZE = 81;
const int WAIT_TIME = 2;

/*
 * PL 4 - Exercise 03
 */
int main(int argc, char *argv[])
{
	// Create mutual exclusion semaphore
	sem_t *mutex;
	mutex = sem_open(SEM_NAME, O_CREAT, S_IRUSR|S_IWUSR, 1);
	if (mutex == SEM_FAILED)
	{
		perror("Semaphore failed.\n");
		exit(EXIT_FAILURE);
	}
	
	// Get filename
	char filename[FILENAME_SIZE];
	if ((argc > 1))
	{
		strcpy(filename, argv[1]);
	} else
	{
		strcpy(filename, DEFAULT);
	}

	/******************************************************************/
	sem_wait(mutex); // Lock semaphore
	/************************* CRITICAL AREA **************************/
	
	// Open File with append privileges 
	FILE *fptr = fopen(filename, "a");
	if (fptr == NULL)
	{
		perror("Invalid file.\n");
		exit(EXIT_FAILURE);
	}
	
	// Append to file
	fprintf(fptr, "Eu sou o processo com o PID %d.\n", getpid());
	// Close file
	fclose(fptr);
	
	// Sleep
	sleep(WAIT_TIME);
	
	/************************* CRITICAL AREA **************************/
	sem_post(mutex); // Unlock semaphore
	/******************************************************************/

	printf("append msg to file (%s)", filename);

	// Close Semaphore
	if (sem_close(mutex) < 0)
	{
		perror("SEM close failed.\n");
		exit(EXIT_FAILURE);
	}
	printf("\nSEM closed with success.\n");
	
	// Unlink semaphore option
	if ((argc > 2) && (strcmp(UNLINK_CMD, argv[2]) == 0))
	{
		if (sem_unlink(SEM_NAME) < 0)
		{
			perror("SEM unlink failed.\n");
			exit(EXIT_FAILURE);
		}
		printf("SEM unlinked with success.\n");
	}

	return 0;
}
