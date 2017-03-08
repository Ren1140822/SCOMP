/*
 * Renato Oliveira > 1140822@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * write_vector.c
 * 
 */
#include <stdio.h>
#include <stdlib.h>

#define STEP 200000

/*
 * Writes a part of the vector on the screen.
 * 
 */
void write_vector(int *vec,int start_pos)
{
	int i;
	for(i=start_pos;i<start_pos+STEP;i++)
	{
		printf("%d\n",*(vec+i));
	}
	
}
