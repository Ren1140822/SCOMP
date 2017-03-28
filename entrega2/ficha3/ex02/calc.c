/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * calc.c
 * 
 */
#include <stdio.h>

/*
 * Calculate the average of a vector of integers.
 */ 
float average(int *vector, unsigned int v_size)
{
	// If size 0 (Avoid DIV BY 0)
	if (v_size == 0)
	{
		return 0;
	}
	
	int i, sum = 0;
	for (i = 0; i < v_size; i++)
	{
		sum += vector[i];
	}
	return sum / (float) v_size;
}
