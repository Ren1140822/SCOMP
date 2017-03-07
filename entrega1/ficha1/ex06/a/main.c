/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * main.c
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>
#include "print_vec.h"

#define SIZE 1005000
#define MAX_INTEGER 99

/*
 * PL 1 - Exercise 6
 */
int main(void)
{
	
	int vec[SIZE], resultado[SIZE];
	// Intializes random number generator
	time_t t;
	srand((unsigned) time(&t));
	// Random values
	int i;
	for (i = 0; i < SIZE; i++)
	{
		vec[i] = 1;//rand() % MAX_INTEGER;
	}
	
	printf("VEC:\n");
	print_vec(vec, SIZE);
	
	pid_t pid;
	
	pid = fork();
	if (pid < 0)
	{
		perror("Error creating the child process.\n");
		exit(-1);
	}
	
	if (pid > 0)
	{
		for (i = SIZE / 2; i < SIZE; i++)
		{
			resultado[i] = vec[i]*4 + 20;
		}

		// wait for child and then print vec
		wait(NULL);
		// Print second half
		print_vec((resultado + SIZE/2), SIZE/2);
	}
	else
	{
		for (i = 0; i < SIZE / 2; i++)
		{
			resultado[i] = vec[i]*4 + 20;
		}
		// Print first half
		printf("RESULTADOS:\n");
		print_vec(resultado, SIZE/2);
		exit(1);
	}

	return 0;
}
