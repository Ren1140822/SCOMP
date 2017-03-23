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
#include "generate_random.h"


const int READ= 0;
const int WRITE =1;
const int START_BALANCE = 20;
const int PAI_FILHO= 0;
const int FILHO_PAI =1;
const int OPTION_PLAY=1;
const int OPTION_END=0;
const int BALANCE_INC =10;
const int BALANCE_DEC=5;

/*
 * PL 2 - Exercise 09
 */
int main(void)
{
	pid_t pid;
	int fd[2][2]; 			// dois pipes: fd[2][2]
	int read_number;
	int msg;
	int option;
	int pipe_flags=0;
	int balance= START_BALANCE;
	
	
	if(pipe(fd[0]) == -1 || pipe(fd[1])==-1)	// ERRO AO CRIAR PIPE
	{
		perror("pipe failed.\n");
		return -1; 
	}
	
	pid = fork();
	if (pid < 0)		// ERRO AO CRIAR PROCESSO
	{
		perror("fork failed.\n");
		return -1;
	}
	do{
	if (pid > 0) 	// PROCESSO PAI
	{
		//verifica se ja foi fechado as extremidades nao importantes
		if(pipe_flags==0)
		{
			close(fd[PAI_FILHO][READ]);
			close(fd[FILHO_PAI][WRITE]);
			pipe_flags=1;
		}
		//para melhorar o fluxo dos printf
		sleep(2);
		
		printf("Write a number (1-5).\n");
		scanf("%d",&read_number);
		if(read_number>0 && read_number<6)
		{
			//se numero estiver dentro do threshold envia as opçoes para o filho
		   write(fd[PAI_FILHO][WRITE], &OPTION_PLAY, sizeof(int));
		   write(fd[PAI_FILHO][WRITE], &OPTION_END, sizeof(int));
		   //le o numero random do filho
		   read(fd[FILHO_PAI][READ], &msg, sizeof(int));
		   //se filho tiver escolhido sair, vai enviar 0
		  if(msg!=0)
		  {
			//printf do valor obtido e escrito pelo user
			printf("Your number was %d, and you got %d.\n",read_number,msg);
			if(msg==read_number)
			{
				//incrementa saldo e envia pro filho
			   balance+=BALANCE_INC;
			   write(fd[PAI_FILHO][WRITE], &balance, sizeof(int));
			}
		   else
		   {
			   //decrementa e envia para o filho
				balance-=BALANCE_DEC;
				write(fd[PAI_FILHO][WRITE], &balance, sizeof(int));
		   }
		  }
		  else
		  {
			  //faz break do ciclo para poder fechar os descritores e esperar pelo filho
			  break;
		  }
		}
		
	} else 				// PROCESSO FILHO
	{
		//verifica se já foi fechado uma vez, porque ta dentro de um ciclo
		if(pipe_flags==0)
		{
			close(fd[PAI_FILHO][WRITE]);
			close(fd[FILHO_PAI][READ]);
			pipe_flags=1;
		}
		//le a opçao enviada pelo pai
	    read(fd[PAI_FILHO][READ], &option, sizeof(int)); 
	    printf("Press %d to bet | ",option);
	    read(fd[PAI_FILHO][READ], &option, sizeof(int)); 
	    printf("Press %d to quit.\n",option);
	    //le a opçao escolhida
	    scanf("%d",&msg);
		if(msg==1)
		{
			 //atribui numero random
			 msg= generate_random(1,5);
			 //escreve o numero para o pai
			 write(fd[FILHO_PAI][WRITE], &msg, sizeof(int));
			 //recebe o saldo atualizado
			 read(fd[PAI_FILHO][READ], &balance, sizeof(int)); 
			 printf("Balance:%d Euros.\n",balance);
		}
		else
		{
			//se user escolher sair, envia uma msg especifica para o pai de quit
			write(fd[FILHO_PAI][WRITE], &msg, sizeof(int));
			close(fd[PAI_FILHO][READ]);
			close(fd[PAI_FILHO][WRITE]);
			break;
		}
	}
	}while(balance>0);
	if (pid > 0) 	// PROCESSO PAI
	{
		//fecha fora do ciclo, nao pode ser dentro
		close(fd[PAI_FILHO][WRITE]);
		close(fd[PAI_FILHO][READ]);
		// Esperar que o filho acabe (BOA PRÁTICA)
		wait(NULL);
	}
	return 0;
}
