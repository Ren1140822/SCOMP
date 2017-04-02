/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * vector_helper.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
 * Function to print a vector with n integers.
 * 
 * int *vec - pointer to vector
 * unsigned int n - the number of registers to print. 
 */
void print_vec(int *vec, unsigned int size) 
{
	printf("\n{");
	
	int i;
	for (i = 0; i < size; i++)
	{
		printf("%d%s", vec[i], i != size-1 ? ";": "");
	}
	printf("}\n\n");
}

/*
 * Fills a vector with n random of integers.
 */ 
void fill_with_random_ints(int *vec, unsigned int size, int min, int max)
{
	/* Intializes random number generator */
	time_t t;
	srand((unsigned) time(&t));
	
	int i;
	for (i = 0; i < size; i++)
	{
		vec[i] = rand() % max + min;
	}
}

/*
 * Returns the maximum element of the array.
 */
int find_max(int *vec, unsigned int size) 
{
	if (size < 1)
	{
		return -1;
	}
	
	// assign first
	int max = *vec, i;
	for (i = 1; i < size; i++)
	{
		if (max < vec[i])
		{
			max = vec[i];
		}
	}
	return max;
}
	
	
	
	
	
	
	
	
	
	

