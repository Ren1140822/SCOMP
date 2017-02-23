/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * main.c
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include "copy_string.h"

/*
 * Module 0 - Exercise 6
 */

#define SIZE 10
char *str1 = "Good Morning Vietnam.";
char str2[SIZE];
	
int main(void) {
	
	printf("\n1st string: \"%s\"\n", str1);
	
	copy_string(str1, str2);
	
	printf("\n2nd string: \"%s\"\n", str2);
	
	return 0;
}
