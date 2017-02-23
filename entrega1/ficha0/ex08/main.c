/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * main.c
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include "somabyte2.h"
#include "print_vec.h"

/*
 * Module 0 - Exercise 8
 */

#define SIZE 5
	
int main(void) {
	
	int vec1[SIZE] = {SIZE, 0xFF01, 0xFFFF03FF, 0xFFFF00FF, 1};
	int vec2[SIZE - 1];
	char x = 2;
	
	printf("Vector 1 :\t");
	print_vec(vec1, SIZE);
	
	printf("(RANDOM) Vector 2 :\t");
	print_vec(vec2, SIZE - 1); // Considered that vec2 will not store is SIZE (what I interpreted from the exercise)
	
	somabyte2(x, vec1, vec2);
	
	printf("\n\nAfter method: (char value is %hhd)\n", x);
	
	printf("(UNCHANGED) Vector 1 :\t");
	print_vec(vec1, SIZE);
	
	printf("\n(CHANGED) Vector 2 :\t");
	print_vec(vec2, SIZE - 1);
	
	return 0;
}
