/*
 * Renato Oliveira > 1140822@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * generate_random.c
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int generate_random(int start,int max)
{
	// Intializes random number generator
	time_t t;
	srand((unsigned) time(&t));

	return rand() % max + start;
}
