/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * batota.c
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>

#define MAX_VALUE 5

int main(void)
{
	// Intializes random number generator
	time_t t;
	srand((unsigned) time(&t));

	return rand() % MAX_VALUE + 1;
}
