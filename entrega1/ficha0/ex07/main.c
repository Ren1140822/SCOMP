/*
 * Renato Oliveira > 1140822@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * main.c
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include "palindrome.h"

/*
 * Module 0 - Exercise 7
 */

#define SIZE 17

int main(void) {
	char vec[SIZE] = "never odd or even";
	int resultado= palindromes(vec,SIZE-1);
	 
	  
	if(resultado==1)
	{
		printf("É palindrome.\n");
	}
	else{
		
		printf("Não é palindrome.\n");
	}
	return 0;
}
