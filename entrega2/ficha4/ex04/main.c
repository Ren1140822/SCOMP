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
#include <time.h>
#include <errno.h>

#include "file_helper.h"

// Setting constants
const char *SEM_NAME = "sem_mutex";
const char *DEFAULT = "input.txt";
const int FILENAME_SIZE = 81;
const int WAIT_TIME = 12;
const int MAX_LINES = 10;
const int TIMEOUT = 10;

const char *UNLINK_CMD = "-u";
const char *REMOVE_LINE_CMD = "-rl";
const char *CLEAR_FILE_CMD = "-c";

/*
 * PL 4 - Exercise 04
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
	struct timespec ts;
	if (clock_gettime(CLOCK_REALTIME, &ts) == -1)
	{
		perror("close_gettime failed.\n");
		exit(EXIT_FAILURE);
	}
	ts.tv_sec += TIMEOUT;
	if (sem_timedwait(mutex, &ts) == -1) // Trys to lock (decrement) before TIMEOUT
	{
		if (errno == ETIMEDOUT)
		{
			printf("timed out.\n");
			exit(EXIT_FAILURE);
		} else
		{
			perror("sem_timedwait failed.\n");
			exit(EXIT_FAILURE);
		}
	}
	/************************* CRITICAL AREA **************************/
	
	// Truncate file
	if ( ( (argc > 2) && (strcmp(CLEAR_FILE_CMD, argv[2]) == 0) ) ||
		 ( (argc > 3) && (strcmp(CLEAR_FILE_CMD, argv[3]) == 0) ) || 
		 ( (argc > 4) && (strcmp(CLEAR_FILE_CMD, argv[4]) == 0) ) )
	{
		if (truncate(filename, 0) < 0)
		{
			sem_post(mutex); // Unlock semaphore
			perror("truncate file failed.\n");
			exit(EXIT_FAILURE);
		}
		printf("File truncated.\n");
		
	} else if ( ( (argc > 2) && (strcmp(REMOVE_LINE_CMD, argv[2]) == 0) ) ||  // Remove first line option
			    ( (argc > 3) && (strcmp(REMOVE_LINE_CMD, argv[3]) == 0) ) || 
			    ( (argc > 4) && (strcmp(REMOVE_LINE_CMD, argv[4]) == 0) ) )
	{
		if (remove_line(filename) < 0)
		{
			sem_post(mutex); // Unlock semaphore
			perror("Remove line failed.\n");
			exit(EXIT_FAILURE);
		}
		printf("First Line removed.\n");
	}
	
	int no_lines = count_lines(filename);
	if (no_lines < 0)
	{
		sem_post(mutex); // Unlock semaphore
		perror("count lines failed.\n");
		exit(EXIT_FAILURE);
	} else if (no_lines < MAX_LINES)
	{
		// Open File with append privileges 
		FILE *fptr = fopen(filename, "a");
		if (fptr == NULL)
		{
			sem_post(mutex); // Unlock semaphore
			perror("Invalid file.\n");
			exit(EXIT_FAILURE);
		}
		
		// Append to file
		fprintf(fptr, "Eu sou o processo com o PID %d.\n", getpid());
		// Close file
		fclose(fptr);
	} else
	{
		printf("maximum number of lines already written.\n");
	}
	
	// Sleep
	sleep(WAIT_TIME);
	
	/************************* CRITICAL AREA **************************/
	sem_post(mutex); // Unlock semaphore
	/******************************************************************/

	// Close Semaphore
	if (sem_close(mutex) < 0)
	{
		perror("SEM close failed.\n");
		exit(EXIT_FAILURE);
	}
	printf("\nSEM closed with success.\n");
	
	// Unlink semaphore option
	if ( ( (argc > 2) && (strcmp(UNLINK_CMD, argv[2]) == 0) ) ||
		 ( (argc > 3) && (strcmp(UNLINK_CMD, argv[3]) == 0) ) || 
		 ( (argc > 4) && (strcmp(UNLINK_CMD, argv[4]) == 0) ) )
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
