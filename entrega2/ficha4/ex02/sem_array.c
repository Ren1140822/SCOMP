/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * sem_array.c
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

// Constants
const char *SEM_NAME = "sem_no_";
const size_t BUFF_SIZE = 81;

void create_sem_array(sem_t **sems, size_t SIZE, int sem_value)
{
	int i;
	for (i = 0; i < SIZE; i++)
	{
		// Create SEM name
		char tmp[SIZE];
		snprintf(tmp, BUFF_SIZE, "%s%d", SEM_NAME, (i+1));
		// Open SEM
		sems[i] = sem_open(tmp, O_CREAT | O_EXCL, S_IRUSR|S_IWUSR, sem_value);
		if (sems[i] == SEM_FAILED)
		{
			perror("Semaphores failed.\n");
			exit(EXIT_FAILURE);
		}
	}
}

void unlink_sem_array(sem_t **sems, size_t SIZE)
{
	int i;
	for (i = 0; i < SIZE; i++)
	{
		// Create SEM name
		char tmp[SIZE];
		snprintf(tmp, BUFF_SIZE, "%s%d", SEM_NAME, (i+1));
		// Unlink Semaphore
		if (sem_unlink(tmp) < 0)
		{
			perror("SEM unlink failed.\n");
			exit(EXIT_FAILURE);
		}
	}
}
