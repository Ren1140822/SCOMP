/*
 * Renato Oliveira > 1140822@isep.ipp.pt
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
#include "shd_type.h"

#define NUM_SEMS 2

const char *SEM_NAME[NUM_SEMS] = {"sem_request", "sem_process"};
const int NR_SEM_REQUEST =0;
const int NR_SEM_PROCESS =1;

typedef struct
{
	int client_number;
	int balance;
}
account;


/*
 * PL 4 - Exercise 08
 */
int main(int argc, char *argv[])
{
	// Create mutual exclusion semaphore

	int fd, data_size = sizeof(shd_type);
	shd_type *sh_data;
	sem_t *sems[NUM_SEMS];
	account acc;
	sems[NR_SEM_REQUEST] = sem_open(SEM_NAME[NR_SEM_REQUEST], O_CREAT | O_EXCL, S_IRUSR|S_IWUSR, 1); 
	sems[NR_SEM_PROCESS] = sem_open(SEM_NAME[NR_SEM_PROCESS], O_CREAT | O_EXCL, S_IRUSR|S_IWUSR, 0); 
	if (sems[NR_SEM_REQUEST] == SEM_FAILED || sems[NR_SEM_PROCESS] == SEM_FAILED)
	{
		perror("Semaphore failed.\n");
		exit(EXIT_FAILURE);
	}
	acc.client_number =1;
	acc.balance = 2000;
	// Open shm
	fd = shm_open(SHM_NAME, O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
	if (fd < 0)
	{
		perror("Error opening shared memory.");
		exit(EXIT_FAILURE);
	}
	// Truncate shm
	if (ftruncate(fd, data_size) < 0)
	{
		perror("Truncating shared memory failed.");
		exit(EXIT_FAILURE);
	}
	// Map shm
	sh_data = (shd_type *) mmap(NULL, data_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if (sh_data == MAP_FAILED)
	{
		perror("Mapping shared memory failed.");
		exit(EXIT_FAILURE);
	}
	
	//main server loop
	while(1)
	{
		int sval;
		sem_getvalue(sems[NR_SEM_REQUEST],&sval);
		if(sval<1) //there was a request for account info or withdrawal.
		{
			int i=0;
			for (i = 0; i < 2; i++)
			{
				if(sh_data->client_number == acc.client_number)
				{
					sh_data->balance = acc.balance;
					sem_post(sems[NR_SEM_PROCESS]);
				}
			}
		}
	}
	
	// Close Semaphore
	int i;
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
	// Unmap & close
	if (munmap(sh_data, data_size) < 0)
	{
		exit(EXIT_FAILURE);
	}
	if (close(fd) < 0)
	{
		exit(EXIT_FAILURE);
	}
	if (shm_unlink(SHM_NAME) < 0)
	{
		exit(EXIT_FAILURE);
	}
	return 0;
}
