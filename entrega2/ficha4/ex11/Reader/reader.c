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


#define NUM_SEMS 6

const char *SEM_NAME[NUM_SEMS] = {"sem_mutex2", "sem_r","sem_w","sem_readers","sem_writers","sem_mutex_writers"};
const int MUTEX2 =0;
const int R =1;
const int W=2;
const int NR_READERS =3;
const int WRITERS=4;
const int MUTEX_NR_WRITERS=5;




/*
 * PL 4 - Exercise 11
 */
int main(int argc, char *argv[])
{

	int fd, data_size = sizeof(shd_type);
	shd_type *sh_data;
	sem_t *sems[NUM_SEMS];
	sems[W] = sem_open(SEM_NAME[W], O_CREAT , S_IRUSR|S_IWUSR, 1);
	sems[R] = sem_open(SEM_NAME[R], O_CREAT, S_IRUSR|S_IWUSR, 1);
	sems[MUTEX2] = sem_open(SEM_NAME[MUTEX2],  O_CREAT, S_IRUSR|S_IWUSR, 1); 
	sems[NR_READERS] = sem_open(SEM_NAME[NR_READERS],  O_CREAT, S_IRUSR|S_IWUSR, 5); 
	sems[WRITERS] = sem_open(SEM_NAME[WRITERS],  O_CREAT, S_IRUSR|S_IWUSR, 0); 
	sems[MUTEX_NR_WRITERS] = sem_open(SEM_NAME[MUTEX_NR_WRITERS],  O_CREAT, S_IRUSR|S_IWUSR, 1); 
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

	sem_wait(sems[NR_READERS]);//allow only 5 readers
	while(1)
	{		
		int sval;
		sem_getvalue(sems[WRITERS],&sval);
		if(sval>2)
		{
			sem_wait(sems[MUTEX_NR_WRITERS]);//if more than 2 writers are waiting block the reader
		}
		sem_wait(sems[MUTEX2]);
		
		sh_data->number_readers++;
		if(sh_data->number_readers==1)
		{
				sem_wait(sems[R]);//if its the first reader blocks the writer
		}
		sem_post(sems[MUTEX2]);
		
		sem_wait(sems[W]);
		printf("Read: %s.\n", sh_data->string);
		printf("Number of readers is %d.\n",sh_data->number_readers);
		sem_post(sems[W]);
		
		sem_wait(sems[MUTEX2]);
		
		sh_data->number_readers--;
		if(sh_data->number_readers==0)
		{
				sem_post(sems[R]);
		}
		sem_post(sems[MUTEX2]);
		
	}
	sem_post(sems[NR_READERS]);//when reader finishes cycle, free up a space in the reader queue of 5
	
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
