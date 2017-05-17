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

#include "thread_func.h"
#include "info.h"

// Setting constants
#define SIZE 5
#define NAME_VEC_SIZE 10
const char names[NAME_VEC_SIZE][BUFFER_SIZE] = {"Renato", "Daniel", "Paulo", "José", "Ana", "Ferro", "Eurico", "Eureka", "Spock", "Cocas"};

/*
 * PL 5 - Exercise 01
 */
int main(int argc, char *argv[])
{
	// Intializes random seed
	srand((unsigned) getpid());
	
	// Fill vec info 
	info vec[SIZE];
	int i;
	for (i = 0; i < SIZE; i++)
	{
		vec[i].number = i + 1;
		
		/* Intializes random integer */
		unsigned int random_name = (rand() % NAME_VEC_SIZE) + 0;
		strncpy(vec[i].name, names[random_name], BUFFER_SIZE);
		
		strncpy(vec[i].address, "Areosa no. 99", BUFFER_SIZE);
	}
	
	// Create Threads
	pthread_t threads[SIZE];
    int ret;
    for (i = 0; i < SIZE; i++)
    {
		printf("Creating thread no. %d\n", (i + 1));
		ret = pthread_create(&threads[i], NULL, print_info, (void *) &vec[i]);
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
	
	return 0;
}
