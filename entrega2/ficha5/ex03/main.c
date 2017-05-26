/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
 * Renato Oliveira > 1140822@isep.ipp.pt
 * 
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
#include <pthread.h>
#include <string.h>

#include "info.h"

// Setting constants
#define SIZE 5
#define VEC_SIZE 1000
int vec[VEC_SIZE];
int sum;
info information[SIZE];
pthread_mutex_t mux;
void * func(void *param)
{
	int *index = (int *)param;
	int local_sum=0;
	int magnitude = VEC_SIZE/SIZE;
	int i;
	for (i = (*index)*magnitude; i < ((*index)+1)*magnitude; i++)
	{
		local_sum += vec[i];
	}
	pthread_mutex_lock(&mux);
	sum+=local_sum;
	pthread_mutex_unlock(&mux);
	printf("local sum is %d on index %d with magnitude %d\n",local_sum,*index,(((*index)+1)*magnitude));
	pthread_exit(NULL);
}



/*
 * PL 5 - Exercise 3
 */
int main(int argc, char *argv[])
{
	
	
	pthread_mutex_init(&mux,NULL);
	int i;
	//Fill vector
	for (i = 0; i < VEC_SIZE; i++)
	{
		 vec[i] = 1;
	}
	
	// Create Threads
	pthread_t threads[SIZE];
    int ret;
    for (i = 0; i < SIZE; i++)
    {
		information[i].thread_no = i;
		ret = pthread_create(&threads[i], NULL, func, (void *)&information[i]);
		if (ret != 0) 
		{
			perror("Create thread failed.\n");
			exit(-1);
		}
		
	}
	
	for (i = 0; i < SIZE; i++)
	{
		pthread_join(threads[i], NULL); // Joins with each terminated thread (by order array)
	}
	printf("Total sum is %d.\n",sum);
	
	return 0;
}
