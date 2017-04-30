/*
 * Renato Oliveira > 1140822@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * reader.c
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

#define NUM_SEMS 4

const char *SEM_NAME[NUM_SEMS] = {"sem_mutex2", "sem_r","sem_w","mutex_write"};
const int MUTEX2 =0;
const int R =1;
const int W=2;
const int MUTEX_WRITE=3;
const int LOOPS_NUMBER = 100000;




/*
 * PL 4 - Exercise 10
 */
int main(int argc, char *argv[])
{

	int fd, data_size = sizeof(shd_type);
	shd_type *sh_data;
	sem_t *sems[NUM_SEMS];
	sems[W] = sem_open(SEM_NAME[W], O_CREAT , S_IRUSR|S_IWUSR, 1);
	sems[R] = sem_open(SEM_NAME[R], O_CREAT, S_IRUSR|S_IWUSR, 1);
	sems[MUTEX2] = sem_open(SEM_NAME[MUTEX2],  O_CREAT, S_IRUSR|S_IWUSR, 1); 
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
	
	//main loop

	
	int nr;
	for (nr = 0; nr < LOOPS_NUMBER; nr++) 
	{		
		sem_wait(sems[MUTEX2]);
		
		sh_data->number_readers++;
		if(sh_data->number_readers==1)
		{
				sem_wait(sems[W]);//if its the first reader blocks the writer
				sem_wait(sems[MUTEX_WRITE]);
		}
		sem_post(sems[MUTEX2]);
		
		
		printf("Read: %s.\n", sh_data->string);
		printf("Number of readers is %d.\n",sh_data->number_readers);
		
		
		sem_wait(sems[MUTEX2]);
		
		sh_data->number_readers--;
		if(sh_data->number_readers==0)
		{
				sem_post(sems[W]);//each reader when it finishes reading, decreases the read counter and checks if its 0. if its 0, unblocks this semaphore
				sem_post(sems[MUTEX_WRITE]);
				printf("Writer will enter.\n");
		}
		sem_post(sems[MUTEX2]);
		
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
