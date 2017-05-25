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

// Setting constants
#define SIZE 1000
#define NUM_THREADS 5

// Error constants
const int THREAD_CREATE_ERROR = -1;
const int THREAD_JOIN_ERROR = -2;
const int THREAD_EXIT_ERROR = -3;

// Info Struct
typedef struct
{
	int *vec_ptr;
	int *num_ptr;
	int thread_no;
}
param;

// thread function
void *find(void *params);

/*
 * PL 5 - Exercise 02
 */
int main(int argc, char **argv)
{	
	// Intit variables
	int vec[SIZE];
	int number_to_find;
	// Get search number
	number_to_find = 1; // DEFAULT
	if (argc > 1)
	{
		number_to_find = atoi(argv[1]);
	}
	// Fill vec info 
	int i;
	for (i = 0; i < SIZE; i++)
	{
		vec[i] = i + 1;
	}
	// Create Threads
	pthread_t threads[NUM_THREADS];
	param params[NUM_THREADS]; 
    for (i = 0; i < NUM_THREADS; i++)
    {
		params[i].vec_ptr = vec;
		params[i].num_ptr = &number_to_find;
		params[i].thread_no = i + 1;
		if (pthread_create(&threads[i], NULL, find, (void *) &params[i]) != 0) 
		{
			perror("Create thread failed.\n");
			exit(THREAD_CREATE_ERROR);
		}
	}
	// Joins with each terminated thread (by array order)
	int *ret;
	int ret_val = -1;
	for (i = 0; i < NUM_THREADS; i++)
	{
		if (pthread_join(threads[i], (void *) &ret) != 0) 
		{
			perror("Join thread failed.\n");
			exit(THREAD_JOIN_ERROR);
		}
		if (ret != NULL)
		{
			ret_val = *ret;
		}
		free(ret);
	}
	// Print result
	printf("\n********************************************\n\n");
	if (ret_val == -1)
	{
		printf("Number (%d) wasn't found.\n", number_to_find);
		
	} else
	{
		printf("The thread #%d found the number (%d).\n", ret_val, number_to_find);
	}
	
	return 0;
}

// thread function
void *find(void *params)
{
	param *info = (param *) params;
	
	int *vec = info->vec_ptr;
	int number_to_find = *(info->num_ptr);
	int *index = (int *) malloc(sizeof(int));
	*index = info->thread_no;
	
	int i, magnitude = SIZE / NUM_THREADS;
	for (i = ((*index - 1) * magnitude); i < (*index * magnitude); i++)
	{
		if (vec[i] == number_to_find)
		{
			printf("The number (%d) was found at the index %d\n", number_to_find, i);
			pthread_exit((void *)index);
		}
	}
	pthread_exit(NULL);
}
