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
#include "print_vec.h"
#include "create_childs.h"

#define SIZE 100000
#define NUM_CHILDS 5

/*
 * PL 1 - Exercise 10
 */
int main(void)
{
	// Create vector
	int vec[SIZE];
	int i;
	for (i = 0; i < SIZE; i++)
	{
		vec[i] = i;
	}
	// Print the vector
	printf("VEC:");
	print_vec(vec, SIZE);
	
	// the number to search
	int number = 20000;
	// creates childs and returns the sequence # of creation
	int sequence = create_childs(NUM_CHILDS);
	// treat error
	if (sequence == -1)
	{
		perror("fork failed.\n");
		exit(-1);
	}
	// Child processes
	if (sequence > 0)
	{
		// search magnitude
		int magnitude = SIZE / 5;
		// Each child should iterate one interval
		for(i = magnitude * (sequence - 1); i < magnitude * sequence; i++)
		{
			if (vec[i] == number)
			{
				// If this child finds the number, returns its order sequence
				printf("The index #%d of the vector stores the desired number (%d).\n", i, number);
				exit(sequence);
			}
		}
		// If it doesn't find the number returns 0
		exit(0);
	}
	// Parent process
	int status;
	// Wait for each child (no order)
	for(i = 0; i < NUM_CHILDS; i++)
	{
		wait(&status);
		if (WIFEXITED(status) && (WEXITSTATUS(status) > 0))
		{
			printf("The child #%d found the number.\n", WEXITSTATUS(status));
		}
	}

	return 0;
}
