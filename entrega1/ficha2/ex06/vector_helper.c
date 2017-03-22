/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * vector_helper.c
 */
 
#include <stdio.h>

/*
 * Function to print a vector with n number of integers.
 * int *vec - the vetor to print.
 * unsigned int n - the number of integers to print. 
 */
void print_vec(int *vec, unsigned int n) 
{
	printf("\n{");
	
	int i;
	for (i = 0; i < n; i++)
	{
		printf("%d%s", vec[i], i != n-1 ? "; ": "");
	}
	printf("}\n");
}

/*
 * Fills vector with a n sequence of integers.
 */ 
void fill_vector(int *vec, int n)
{
	int i;
	
	for (i = 0; i < n; i++)
	{
		vec[i] = i + 1;
	}
}
