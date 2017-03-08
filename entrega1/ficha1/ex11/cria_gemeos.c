/*
 * Renato Oliveira > 1140822@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * operation.c
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define NUM_CHILDS 2
/*
 * Função para criar gémeos
 * 
 */
char cria_gemeos(pid_t lista[2])
{
	
	
	
	int i;
	for(i=0;i < NUM_CHILDS; i++)
	{
		
		lista[i]=fork();
		if(lista[i]<0)
		{
			perror("fork failed");
			exit(-1);
		}
		if(lista[i]==0)
		{
			if(i==0)
			{
				return 'a';
			}
			if(i==1)
			{	
				
				return 'b';
			}
		}
	}
	return 'p';
}
