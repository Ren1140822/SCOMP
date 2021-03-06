/*
 * Renato Oliveira > 1140822@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * writer.c
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
#include <string.h>
#include <time.h>

#define NUM_SEMS 5

const char *SEM_NAME[NUM_SEMS] = {"sem_mutex1","sem_mutex3", "sem_r","sem_w","mutex_write"};
const int MUTEX1 =0;
const int MUTEX3 =1;
const int R =2;
const int W=3;
const int MUTEX_WRITE =4;
const int LOOPS_NUMBER = 100000;



/*
 * PL 4 - Exercise 10
 */
int main(int argc, char *argv[])
{
	// Create mutual exclusion semaphore

	int fd, data_size = sizeof(shd_type);
	shd_type *sh_data;
	sem_t *sems[NUM_SEMS];
	time_t rawtime;
	struct tm * timeinfo;
	
	sems[W] = sem_open(SEM_NAME[W], O_CREAT , S_IRUSR|S_IWUSR, 1);
	sems[R] = sem_open(SEM_NAME[R], O_CREAT , S_IRUSR|S_IWUSR, 1);
	sems[MUTEX1] = sem_open(SEM_NAME[MUTEX1],  O_CREAT, S_IRUSR|S_IWUSR, 1); 
	sems[MUTEX3] = sem_open(SEM_NAME[MUTEX3],  O_CREAT, S_IRUSR|S_IWUSR, 1); 
	sems[MUTEX_WRITE] = sem_open(SEM_NAME[MUTEX_WRITE],  O_CREAT, S_IRUSR|S_IWUSR, 1); 
	int i;
	for (i = 0; i < NUM_SEMS; i++) 
	{
		if (sems[i] == SEM_FAILED)
		{
			perror("Semaphore failed.\n");
			exit(EXIT_FAILURE);
		}
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
	
	//main loop

	
	int nr;
	for (nr = 0; nr < LOOPS_NUMBER; nr++) 
	{			
		sem_wait(sems[MUTEX3]);	 //mutual exclusion semaphore, only one writer at the same time.
		
		sem_wait(sems[W]); 
		 
		sem_wait(sems[MUTEX1]);
		
		sh_data->number_writers++;
		if(sh_data->number_writers==1)//if nr of writers is 1
		{
				sem_wait(sems[R]);
		}
		sem_post(sems[MUTEX1]);
		sem_post(sems[W]);
		sem_post(sems[MUTEX3]);	
		sem_wait(sems[MUTEX_WRITE]);
		time(&rawtime);
		timeinfo = localtime(&rawtime);
		sprintf(sh_data->string,"%d\n",(int)getpid());
		strcat(sh_data->string,asctime (timeinfo));
		printf("Number of readers is %d.\n",sh_data->number_readers);
		printf("Number of writers is %d.\n",sh_data->number_writers);
		sem_post(sems[MUTEX_WRITE]);
		sem_wait(sems[MUTEX1]);
		sh_data->number_writers--;
		if(sh_data->number_writers==0)
		{
				sem_post(sems[R]);
		}
		sem_post(sems[MUTEX1]);
		
	}
	
	// Close Semaphore
	
	for (i = 0; i < NUM_SEMS; i++) // Close & unlink both SEM
	{
		// Close Semaphore
		if (sem_close(sems[i]) < 0)
		{
			perror("SEM close failed.\n");
			exit(EXIT_FAILURE);
		}
			// Unlink option
		if ((argc > 1) && (strcmp("-rm", argv[1]) == 0))
		{
			// Unlink Semaphore
			if (sem_unlink(SEM_NAME[i]) < 0)
			{
				perror("SEM unlink failed.\n");
				exit(EXIT_FAILURE);
			}
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
	// Unlink option
	if ((argc > 1) && (strcmp("-rm", argv[1]) == 0))
	{
		if (shm_unlink(SHM_NAME) < 0)
		{
			exit(EXIT_FAILURE);
		}
	}
	return 0;
}
