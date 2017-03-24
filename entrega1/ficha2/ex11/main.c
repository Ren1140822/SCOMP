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
const int NUMERO_FILHOS =4;
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
			produtos[i].codigo_barras=10000+i;
			char * id_prod="Produto_";
			char * full_description = concat_string_with_int(id_prod,i+1);
			
			strncpy(produtos[i].descricao,full_description,BUFFER_SIZE);
			
			produtos[i].preco=0.1f;
			
		}
		
		close(pipe_principal[WRITE]);
		struct MENSAGEM_PEDIDO mensagem_pedido_temp;
		while(read(pipe_principal[READ], &mensagem_pedido_temp, sizeof(mensagem_pedido_temp)))
		{ 
			//variavel de controlo para verificar se foi encontrado ou nao um produto.
	     	int was_found;
	     	
			//PESQUISAR ARRAY
			for(i=0;i<NUMERO_PRODUTOS;i++)
			{
				//reinicio de variavel por causa dos outros filhos
				was_found=0;
				if(mensagem_pedido_temp.codigo_barras == produtos[i].codigo_barras)
				{
					//se encontrou o codigo de barras, formula a resposta com o produto certo.
					write(pipes_filhos[mensagem_pedido_temp.id_filho-1][WRITE],&produtos[i],sizeof(produtos[i]));
					was_found=1;
					break;
					
				}
				
				
			}
			if(!was_found)
			{
				printf("Não foi encontrado um match para o codigo %d enviado pelo filho %d.\n",mensagem_pedido_temp.codigo_barras,mensagem_pedido_temp.id_filho);
				//Se nao encontrou, cria uma resposta no mesmo formato da estrutura, onde o codigo de barras será -1, para posterior verificaçao do filho. 
				//Isto é necessario porque o filho precisa de ler algo no formato da estrutura.
				struct MENSAGEM_RESPOSTA resposta_saida;
				resposta_saida.codigo_barras=-1;
				resposta_saida.preco=0.0f;
				write(pipes_filhos[mensagem_pedido_temp.id_filho-1][WRITE],&resposta_saida,sizeof(resposta_saida));
				
				
			}
			
	    }
	    //Fecha o pipe principal extremidade de leitura
		close(pipe_principal[READ]);
		//Fecha a escrita para todos os filhos e espera que eles acabem
		for(i =0;i<NUMERO_FILHOS;i++)
		{
			{
				close(pipes_filhos[i][WRITE]);
				wait(NULL);
	        }
		}
		
		
		
	} 
	else 				// PROCESSO FILHO
	{
		close(pipes_filhos[index_processo-1][WRITE]);
		
		close(pipe_principal[READ]);
		struct MENSAGEM_PEDIDO mensagem_pedido1;
		struct MENSAGEM_RESPOSTA resposta_obtida;
		mensagem_pedido1.id_filho=index_processo;
		printf("Sou o filho %d, insira um código de barras a pesquisar.\n",index_processo);
		int codigo;
		scanf("%d",&codigo);
		mensagem_pedido1.codigo_barras=codigo;
		write(pipe_principal[WRITE], &mensagem_pedido1, sizeof(mensagem_pedido));
		read(pipes_filhos[index_processo-1][READ],&resposta_obtida,sizeof(resposta_obtida));
		if(resposta_obtida.codigo_barras!=-1)
		{
			printf("Filho %d encontrou um match.\n",index_processo);
			printf("---Nome produto: %s.\n",resposta_obtida.descricao);
			printf("---Preço: %f.\n",resposta_obtida.preco);
			printf("---Codigo de barras: %d.\n",resposta_obtida.codigo_barras);
		}
		close(pipes_filhos[index_processo-1][READ]);
		close(pipe_principal[WRITE]);
		exit(0);
	}
	
	return 0;
}
