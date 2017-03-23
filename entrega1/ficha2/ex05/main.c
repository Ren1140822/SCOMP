/*
 * Renato Oliveira > 1140822@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * main.c
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include "preenche_vetor.h"
#include "create_childs.h"

const int READ=0;
const int WRITE=1;
const int VEC_WRITE_STEP =200;
// const int PAI_FILHO 0;
// const int FILHO_PAI 1;

/*
 * PL 2 - Exercise 05
 */
int main(void)
{
	
	int fd[2]; 			// dois pipes: fd[2][2]
	int vec1[1000];
	int vec2[1000];
	int msg;
	preenche_vetor(vec1,1000);
	preenche_vetor(vec2,1000);
	
	if(pipe(fd) == -1)	// ERRO AO CRIAR PIPE
	{
		perror("pipe failed.\n");
		return -1; 
	}
	
	int index_processo = create_childs(5);
	
    if (index_processo ==  0) 	// PROCESSO PAI
	{
		close(fd[WRITE]);
		
		int total_sum=0;
		  while(read(fd[READ], &msg, sizeof(int)))
		 {
		 		total_sum+=msg;
		 }
		
		close(fd[READ]);
		printf("Total sum is:%d.\n",total_sum);
		// Esperar que o filho acabe (BOA PRÁTICA)
		wait(NULL);
		
	} else 				// PROCESSO FILHO
	{
		close(fd[READ]);
		int sum=0,i;
		//multiplicar o meu index de processo(1 a 5) 
		for(i=VEC_WRITE_STEP*(index_processo-1);i<(VEC_WRITE_STEP*(index_processo-1))+VEC_WRITE_STEP;i++)
		{
			sum += *(vec1+i)+*(vec2+i);
		}
		write(fd[WRITE], &sum, sizeof(int));
		
		
		close(fd[WRITE]);
	}
	
	return 0;
}
