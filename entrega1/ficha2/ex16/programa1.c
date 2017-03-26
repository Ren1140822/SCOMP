/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * programa1.c
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

const int NUM_ITS = 5;
const int MAX_VALUE = 10;

/*
 * PL 2 - Exercise 16 - Programa 1
 */
int main(void)
{
	// Intializes random number generator
	time_t t;
	srand((unsigned) time(&t));
	
	int i;
	for (i = 0; i < NUM_ITS; i++)
	{
		printf("%d\n", rand() % MAX_VALUE + 1);
		sleep(1);
	}
	
	printf("0\n");
	
	return 0;
}
