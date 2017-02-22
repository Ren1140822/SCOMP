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
	if(size==0 )
	{
		return 0;
	}
	else if(*vec % 2 ==0)
	{
		return *vec+somar_pares(vec+1,size-1);
	}
	return somar_pares(vec+1,size-1);
}
