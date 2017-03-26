/*
 * Renato Oliveira > 1140822@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * fatorial.c
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

void fatorial()
{
    int numero,i;
	
	//le do stdin
	read(STDIN_FILENO,&numero,sizeof(int));
	for(i=numero;i>1;i--)
	{
		numero = numero * (i-1);
		
	}
    //em vez de printf para o stdout, faz se um write, para evitar trocas de tipos de variaveis
	write(STDOUT_FILENO,&numero,sizeof(int));
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	exit(0);
}
