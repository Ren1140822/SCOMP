/*
 * Renato Oliveira > 1140822@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * preenche_vetor.c
 * 
 */
#include <stdio.h>


void preenche_vetor(int *vec,int size)
{
	int i;
	for(i=0;i<size;i++)
	{
		*(vec+i)=i+1;
	}
	
}
