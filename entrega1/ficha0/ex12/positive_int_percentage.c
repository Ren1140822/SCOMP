/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * positive_int_percentage.c
 * 
 */
#include <stdio.h>
#include <stdlib.h>

/*
 * Implemente uma função que leia do teclado uma sequência de números 
 * terminada pela introdução do número 0. Pretende-se saber qual a percentagem 
 * de números positivos introduzidos. Utilize um vector dinâmico para armazenar 
 * os valores introduzidos.
 */
float positive_int_percentage(int *vec, unsigned int *size)
{
	unsigned int i = 0, positive_num = 0; 
	int temp;
	*size = 0;
	
	printf("Insert elements:\n");
		
	do
	{
		// Read integer
		scanf("%d", &temp);
		if (temp != 0)
		{		
			// Increment size
			(*size)++;
			// Increment if positive
			positive_num += (temp > 0) ? 1 : 0;
			// Reallocate vector to increment size
			int *ptr_temp = NULL;
			ptr_temp = (int *) realloc(vec, (*size) * sizeof(int));
			// check realloc return 
			if (ptr_temp != NULL)
			{
				vec = ptr_temp;
				ptr_temp = NULL;
			}
			// Add element to vector
			vec[i] = temp;
			i++;
		}
	} while (temp != 0);
	
	if (*size == 0)
	{
		return -1;
	} else 
	{
		return positive_num / (float) *size;
	}
	
}
