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
const int ACCS_NUMBER=3;

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
	account * accs = malloc(ACCS_NUMBER*sizeof(account));
	sems[NR_SEM_REQUEST] = sem_open(SEM_NAME[NR_SEM_REQUEST], O_CREAT | O_EXCL, S_IRUSR|S_IWUSR, 0); 
	sems[NR_SEM_PROCESS] = sem_open(SEM_NAME[NR_SEM_PROCESS], O_CREAT | O_EXCL, S_IRUSR|S_IWUSR, 0); 
	if (sems[NR_SEM_REQUEST] == SEM_FAILED || sems[NR_SEM_PROCESS] == SEM_FAILED)
	{
		perror("Semaphore failed.\n");
		exit(EXIT_FAILURE);
	}
	//fills account information for 3 clients
	int j =0;
	for (j = 0; j < ACCS_NUMBER; j++)
	{	
		accs[j].client_number =j+1;
		accs[j].balance = 2000 *(j+1);
	}
	
	
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
	int loop=1;
	
	while(loop)
	{		int found=0;
			sem_wait(sems[NR_SEM_REQUEST]);
			int i=0;
			
			for (i = 0; i < ACCS_NUMBER; i++)
			{
				if(sh_data->client_number == accs[i].client_number)
				{
					//if it finds at least one match.
					found=1;
					
					if(sh_data->op_type==1)//if its a withdrawal
					{
						if(sh_data->withdraw_ammount==0)//break condition
						{
							sem_post(sems[NR_SEM_PROCESS]);
							
							loop=0;
							break;
						}
						if(sh_data->withdraw_ammount<=accs[i].balance)//if it is actually possible to withdraw
						{
							
							accs[i].balance-=sh_data->withdraw_ammount;
						}
					}
					sh_data->balance = accs[i].balance;
					
					sem_post(sems[NR_SEM_PROCESS]);//tells the client the server has finished processing
				}
			
			}
			if(found!=1)//if no client matches found
			{
				printf("Client not found in server.\n");
				
				sh_data->close_connection=1;
				
				sem_post(sems[NR_SEM_PROCESS]);
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
