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
	int number = 5;
	
	// Create childs and returns the sequence # of creation
	int sequence = create_childs(NUM_CHILDS);
	
	// treat error
	if (sequence == -1)
	{
		perror("fork failed\n");
		exit(-1);
	}
	
	// Parent process
	if (sequence == 0)
	{
		int status[NUM_CHILDS];
		// Wait for each child (no order)
		for (i = 0; i < NUM_CHILDS; i++)
		{
			wait(&status[i]);
		}
		// Print only the sequence number of the process that found the number.
		for (i = 0; i < NUM_CHILDS; i++)
		{
			if (status[i] != 0)
			{
				printf("The child #%d found the number(%d)\n", WEXITSTATUS(status[i]), number);
				exit(0); // ************* Verify if any problem ****************
			}
		}
	}
	
	// search magnitude
	int magnitude = SIZE / 5;
	
	// Each child should iterate one interval
	for (i = 1; i <= NUM_CHILDS; i++)
	{
		if (sequence == i)
		{
			int j = magnitude * (sequence - 1);
			while (j < magnitude * sequence)
			{
				if (vec[j] == number)
				{
					// If this child finds the number, returns its order sequence
					printf("The index #%d of the vector stores the desired number (%d)\n", j, number);
					exit(sequence);
				}
				j++;
			}
			// If it doesn't find the number returns 0
			exit(0);
		}
	}
	return 0;
}
