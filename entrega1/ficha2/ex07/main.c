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

const int READ=0;
const int WRITE=1;
const int NUMBER_OF_ROUNDS=10;
#define MESSAGE_SIZE 4
// const int PAI_FILHO 0;
// const int FILHO_PAI 1;
struct JOGO_STRUCT {
	
	int numero_ronda;
	char mensagem[MESSAGE_SIZE];
} jogo;
/*
 * PL 2 - Exercise 07
 */
int main(void)
{
	
	int fd[2]; 			// dois pipes: fd[2][2]
	
	int round_number_win;
	if(pipe(fd) == -1)	// ERRO AO CRIAR PIPE
	{
		perror("pipe failed.\n");
		return -1; 
	}
	
	int index_filho = create_childs(10);
	if (index_filho == 0) 	// PROCESSO PAI
	{
		struct JOGO_STRUCT jogo1;
		close(fd[READ]);
		int i;
		for(i=0;i<NUMBER_OF_ROUNDS;i++)
		{
			jogo1.numero_ronda=i+1;
			strncpy(jogo1.mensagem,"WIN",MESSAGE_SIZE);
			write(fd[WRITE], &jogo1, sizeof(jogo));
			sleep(2);
		}
		close(fd[WRITE]);
		// Esperar que os filhos acabem (BOA PRÁTICA)
		for(i=0;i<NUMBER_OF_ROUNDS;i++)
		{
			pid_t pid_terminado = wait(&round_number_win);
			printf("Filho com pid %d terminou na ronda %d.\n",pid_terminado,WEXITSTATUS(round_number_win));
		}
		
	} else 				// PROCESSO FILHO
	{
		struct JOGO_STRUCT jogo1;
		close(fd[WRITE]);
		read(fd[READ], &jogo1, sizeof(jogo));
		printf("Mensagem %s, lida na ronda %d, pelo filho %d com pid %d.\n",jogo1.mensagem,jogo1.numero_ronda,index_filho,getpid());
		//vai buscar a msg à struct e o pid do filho
		close(fd[READ]);
		exit(jogo1.numero_ronda);
		//faz exit com o valor = ao numero da ronda desta mensagem
		  
		
	}
	
	return 0;
}
