/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * main.c
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "vector_bigger_neighbor.h"
#include "print_vec.h"

/*
 * Module 0 - Exercise 10
 */

#define MAX_INTEGER 50
	
int main(void) {
	
	int *vec_ptr = NULL;
	unsigned int n = 0;
	
	// Ask for the size of the vector
	printf("Insert the size of the vector:\n");
	scanf("%d", &n);
	
	// Intializes a dynamic vector
	vec_ptr = (int *) malloc(n * sizeof(int));
	
	if (vec_ptr == NULL)
	{
		printf("The program encountered an error.");
		exit(EXIT_FAILURE);
	}
	
	// Intializes random number generator
	time_t t;
	srand((unsigned) time(&t));
	
	int i;
	for (i = 0; i < n; i++)
	{
		*(vec_ptr + i) = rand() % MAX_INTEGER;
	}
	
	printf("First Vector:");
	print_vec(vec_ptr, n);
	
	unsigned int peeks_size = 0;
	int *peeks_ptr = vector_bigger_neighbor(vec_ptr, n, &peeks_size);
	
	printf("Second Vector:");
	print_vec(peeks_ptr, peeks_size);
	
	free(vec_ptr);
	free(peeks_ptr);
	
	return 0;
}
