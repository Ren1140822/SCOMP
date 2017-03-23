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
#include "concat_string_with_int.h"


#define BUFFER_SIZE 81

const int READ=0;
const int WRITE=1;
const int NUMERO_FILHOS =3;
const int NUMERO_PRODUTOS=5;

struct MENSAGEM_PEDIDO{
	int id_filho;
	int codigo_barras;
} mensagem_pedido;

struct MENSAGEM_RESPOSTA{
	int codigo_barras;
	float preco;
	char descricao[BUFFER_SIZE];
} mensagem_resposta;
	


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
		int i;
		struct MENSAGEM_RESPOSTA produtos[NUMERO_PRODUTOS];
		for(i=0;i<NUMERO_PRODUTOS;i++)
		{
			produtos[i].codigo_barras=00000+i;
			char * id_prod="Produto ";
			char * full_description = concat_string_with_int(id_prod,i+1);
			strncpy(produtos[i].descricao,full_description,BUFFER_SIZE);
			
			produtos[i].preco=0.1f;
			
		}
		close(pipe_principal[WRITE]);
		struct MENSAGEM_PEDIDO mensagem_pedido_temp;
		while(read(pipe_principal[READ], &mensagem_pedido_temp, sizeof(mensagem_pedido)))
		{ 
	     	printf("filho %d c mensagem %d\n",mensagem_pedido_temp.id_filho,mensagem_pedido_temp.codigo_barras);
	     	
			//PESQUISAR ARRAY
			for(i=0;i<NUMERO_PRODUTOS;i++)
			{
				if(mensagem_pedido_temp.codigo_barras == produtos[i].codigo_barras)
				{
					write(pipes_filhos[mensagem_pedido_temp.id_filho][WRITE],&produtos[i],sizeof(produtos[i]));
					//write(pipes_filhos[mensagem_pedido_temp.id_filho][WRITE],&produtos[i].codigo_barras,sizeof(int));
					//write(pipes_filhos[mensagem_pedido_temp.id_filho][WRITE],&produtos[i].descricao,BUFFER_SIZE);
					//write(pipes_filhos[mensagem_pedido_temp.id_filho][WRITE],&produtos[i].preco,sizeof(float));
				}
				
			}
	    }
		close(pipe_principal[READ]);
		
		// Esperar que o filho acabe (BOA PRÁTICA)
		wait(NULL);
		
	} 
	else 				// PROCESSO FILHO
	{
		close(pipes_filhos[index_processo-1][WRITE]);
		close(pipe_principal[READ]);
		struct MENSAGEM_PEDIDO mensagem_pedido1;
		struct MENSAGEM_RESPOSTA resposta_obtida;
		mensagem_pedido1.id_filho=index_processo;
		mensagem_pedido1.codigo_barras=000002;
		write(pipe_principal[WRITE], &mensagem_pedido1, sizeof(mensagem_pedido));
		read(pipes_filhos[index_processo-1][READ],&resposta_obtida,sizeof(resposta_obtida));
		printf("recebido %s\n",resposta_obtida.descricao);
		close(pipes_filhos[index_processo-1][READ]);
		close(pipe_principal[WRITE]);
	}
	
	return 0;
}
