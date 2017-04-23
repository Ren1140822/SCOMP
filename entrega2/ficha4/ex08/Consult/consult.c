/*
 * Renato Oliveira > 1140822@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * consulta.c
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

/*
 * PL 4 - Exercise 08
 */
int main(int argc, char *argv[])
{
	int client_number,balance;
	int fd, data_size = sizeof(shd_type);
	shd_type *sh_data;
	sem_t *sems[NUM_SEMS];
	// Create mutual exclusion semaphore
	sems[NR_SEM_REQUEST] = sem_open(SEM_NAME[NR_SEM_REQUEST],  O_EXCL, S_IRUSR|S_IWUSR, 1); 
	sems[NR_SEM_PROCESS] = sem_open(SEM_NAME[NR_SEM_PROCESS],  O_EXCL, S_IRUSR|S_IWUSR, 1); 
	if (sems[NR_SEM_REQUEST] == SEM_FAILED || sems[NR_SEM_PROCESS] == SEM_FAILED)
	{
		perror("Semaphore failed.\n");
		exit(EXIT_FAILURE);
	}
	
	fd = shm_open(SHM_NAME, O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
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
	
	printf("Insert client number.\n");
	scanf("%d",&client_number);
	sem_wait(sems[NR_SEM_REQUEST]);
	sh_data->client_number= client_number;
	sem_wait(sems[NR_SEM_PROCESS]);
	balance = sh_data->balance;
	printf("You have %d balance.\n",balance);
	sem_post(sems[NR_SEM_REQUEST]);
	
	
	
	return 0;
}
