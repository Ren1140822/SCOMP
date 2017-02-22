/*
 * Renato Oliveira > 1140822@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * operation.c
 * 
 */
#include <stdio.h>
#include <stdlib.h>

/*
 * 
 * 
 */
int somar_pares(int *vec,unsigned int size)
{
	int i;
	int soma=0;
	for (i = 0;i<size;i++)
	{
		if(vec[i]%2==0)
		{
			soma +=vec[i];
		}
	}
	return soma;
}
