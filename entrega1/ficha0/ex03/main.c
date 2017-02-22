/*
 * Renato Oliveira > 1140822@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * main.c
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include "somar_pares.h"

/*
 * Module 0 - Exercise 3
 */

#define SIZE 5

int main(void) {
	int vec[SIZE] = {1,2,3,4,5};
	int soma= somar_pares(vec,SIZE);
	printf("Resultado da soma é: %d\n",soma);
	return 0;
}
