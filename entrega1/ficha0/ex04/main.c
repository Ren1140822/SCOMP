/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * main.c
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include "operation.h"
#include "print_vec.h"

/*
 * Module 0 - Exercise 4
 */

#define SIZE 3

int main(void) {
	
	int vec[SIZE] = {2,4,6};
	int x = 1;
	
	printf("\nFirst vector:");
	print_vec(vec, SIZE);
	
	printf("The variable to subtract to each element is: %d\n", x);
	
	operation(vec, SIZE, x);
	
	printf("\nModified vector:");
	print_vec(vec, SIZE);
	
	return 0;
}
