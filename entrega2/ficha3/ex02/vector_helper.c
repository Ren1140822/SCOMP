/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * vector_helper.c
 */
#include <stdio.h>

/*
 * Function to print a vector with n integers.
 * 
 * int *vec - pointer to vector
 * unsigned int n - the number of registers to print. 
 */
void print_vec(int *vec, unsigned int n) 
{
	printf("\n{");
	
	int i;
	for (i = 0; i < n; i++)
	{
		printf("%d%s", vec[i], i != n-1 ? ";": "");
	}
	printf("}\n");
}
