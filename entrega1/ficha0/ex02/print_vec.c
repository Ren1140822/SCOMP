/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * print_vec.c
 */
 
#include <stdio.h>

/*
 * Function to print a vector with n number of integers.
 * int *vec - the vetor to print.
 * unsigned int n - the number of integers to print. 
 */
void print_vec(int *vec, unsigned int n) 
{
	printf("\nThe elements of the vector are:\n{ %d", *vec);
	
	unsigned int i = 1;
	while(i < n)
	{
		printf(", %d", *(vec + i));
		i++;
	}
	
	printf(" }\n");
}
