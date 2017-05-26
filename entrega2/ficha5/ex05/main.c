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
int resultado[VEC_SIZE];
int dados[VEC_SIZE];
info information[SIZE];
int counter=0;
pthread_mutex_t mux;
pthread_cond_t thread_cond;
void * func(void *param)
{
	int *index = (int *)param;
	
	int magnitude = VEC_SIZE/SIZE;
	int i;
	pthread_mutex_lock(&mux);
	while(counter!=*index)
	{
		pthread_cond_wait(&thread_cond,&mux);
	}
	for (i = (*index)*magnitude; i < ((*index)+1)*magnitude; i++)
	{
		resultado[i] = dados[i]*2+10;
		printf("Resultado %d:%d.\n",i,resultado[i]);
	}
	counter ++;
	pthread_cond_broadcast(&thread_cond);
	pthread_mutex_unlock(&mux);
	pthread_exit(NULL);
}



/*
 * PL 5 - Exercise 3
 */
int main(int argc, char *argv[])
{
	
	
	pthread_mutex_init(&mux,NULL);
	pthread_cond_init(&thread_cond,NULL);
	int i;
	//Fill vector
	for (i = 0; i < VEC_SIZE; i++)
	{
		 dados[i] = 1*i;
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
	while(counter!=SIZE);
	for (i = 0; i < SIZE; i++)
	{
		pthread_join(threads[i], NULL); // Joins with each terminated thread (by order array)
	}
	pthread_cond_destroy(&thread_cond);
	
	return 0;
}
