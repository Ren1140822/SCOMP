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
#include "create_childs.h"

#define  BUFFER_SIZE 81
const int READ = 0;
const int WRITE =1;
const int NUMERO_FILHOS = 3;
struct MENSAGEM_PEDIDO{
	int id_filho;
	int codigo_barras;
} mensagem_pedido;
struct MENSAGEM_RESPOSTA{
	int codigo_barras;
	float preco;
	char descricao[BUFFER_SIZE];
} mensagem_resposta;
	
// const int PAI_FILHO 0;
// const int FILHO_PAI 1;

/*
 * PL 2 - Exercise 11
 */
int main(void)
{
	int index_processo;
	int pipe_principal[2]; 			// dois pipes: fd[2][2]
	int pipes_filhos[NUMERO_FILHOS][2];
	
	if(pipe(pipe_principal) == -1)	// ERRO AO CRIAR PIPE
	{
		perror("pipe failed.\n");
		return -1; 
	}
	int i;
	for(i =0;i<NUMERO_FILHOS;i++)
	{
		if(pipe(pipes_filhos[i]) == -1)	// ERRO AO CRIAR PIPE
		{
		  perror("pipe failed.\n");
	      return -1; 
		}
	}
	
	  index_processo = create_childs(NUMERO_FILHOS);
	 if (index_processo ==  0) 	// PROCESSO PAI
	{
		
		close(pipe_principal[WRITE]);
		struct MENSAGEM_PEDIDO mensagem_pedido_temp;
		while(read(pipe_principal[READ], &mensagem_pedido_temp, sizeof(mensagem_pedido)))
		{ 
	     	printf("filho %d c mensagem %d\n",mensagem_pedido_temp.id_filho,mensagem_pedido_temp.codigo_barras);
	    }
		
		// write(fd[WRITE], &msg, size);
		
		close(pipe_principal[READ]);
		
		// Esperar que o filho acabe (BOA PRÁTICA)
		wait(NULL);
		
	} 
	else 				// PROCESSO FILHO
	{
		close(pipes_filhos[index_processo-1][WRITE]);
		close(pipe_principal[READ]);
		struct MENSAGEM_PEDIDO mensagem_pedido1;
		mensagem_pedido1.id_filho=index_processo;
		mensagem_pedido1.codigo_barras=1010101;
		write(pipe_principal[WRITE], &mensagem_pedido1, sizeof(mensagem_pedido));
		
		/* Working with strings
		 * while(read(fd[READ], &msg, BUFFER_SIZE))
		 * {
		 * 		do something
		 * }
		*/
		
		
		// read(fd[READ], &msg, size); ||
		/*
		 * while(read(fd[READ], &msg, size))
		 * {
		 * 		do something
		 * }
		*/
		
		close(pipes_filhos[index_processo-1][READ]);
		close(pipe_principal[WRITE]);
	}
	
	return 0;
}
