/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * main.c
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include "positive_int_percentage.h"
#include "print_vec.h"

/*
 * Module 0 - Exercise 12
 */
int main(void) {
	
	int *vec = NULL;
	// Intializes a dynamic vector
	vec = (int *) malloc(sizeof(int));
	if (vec == NULL)
	{
		printf("The program encountered an error.");
		exit(EXIT_FAILURE);
	}
	
	unsigned int size;
	float positive_ints = positive_int_percentage(vec, &size);

	printf("\nThe inserted elements are:");
	print_vec(vec, size);
	printf("\nThe percentage of positive numbers is: %.2f\n", positive_ints);
	
	free(vec);
	
	return 0;
}
