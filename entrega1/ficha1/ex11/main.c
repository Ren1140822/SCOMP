/*
 * Renato Oliveira > 1140822@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * main.c
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include "cria_gemeos.h"

#define NUMERO_GEMEOS 3
#define SIZE_LISTA_COMPLETA 6

/*
 * Module 1 - Exercise 11
 */

int main(void) {
	
	
    
    pid_t lista[SIZE_LISTA_COMPLETA];
    char id;
    int status;
    int i;
    for(i=0;i<NUMERO_GEMEOS;i++)
    {
		id = cria_gemeos(lista+(2*i));
		if(id!='p')
		{
			printf("pid: %d.\n",getpid());
			printf("pid do pai: %d.\n",getppid());
			exit(id);
		}
		
	}
	for(i=0;i<SIZE_LISTA_COMPLETA;i++)
	{
		waitpid(lista[i],&status,0);
		printf("PID do %dº filho: %d, com valor de saida %c.\n",i+1,lista[i],WEXITSTATUS(status));
	}
  
 
	
	
	
	
	return 0;
}
