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
#include "redirect_file_descriptor.h"


const int READ=0;
const int WRITE=1;
// const int PAI_FILHO 0;
// const int FILHO_PAI 1;

/*
 * PL 2 - Exercise 00
 */
int main(void)
{
	pid_t pid;
	int fd[2]; 			// dois pipes: fd[2][2]
	int numero,resultado;
	
	
	if(pipe(fd) == -1)	// ERRO AO CRIAR PIPE
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
	else if (pid > 0) 	// PROCESSO PAI
	{
		
		 printf("Insira um numero.\n");
		 scanf("%d",&numero);
		 //escrever o numero no pipe
		 write(fd[WRITE],&numero,sizeof(int));
		 //é preciso este sleep senao em vez de ler a resposta do meu programa, vou ler o que eu pus no pipe-
		 sleep(2);
		 //ler o resultado do fatorial
		 read(fd[READ],&resultado,sizeof(int));
		 printf("Resultado é %d.\n",resultado);
		 close(fd[WRITE]);
		 close(fd[READ]);
		
		// Esperar que o filho acabe (BOA PRÁTICA)
		wait(NULL);
		
	} 
	else 				// PROCESSO FILHO
	{
		 //alterar os descritores do processo que vou chamar
		 redirect_file_descriptor(fd[READ],STDIN_FILENO);
		 redirect_file_descriptor(fd[WRITE],STDOUT_FILENO);
		 execlp("fatorial/exec","exec",(char*)NULL);
		
	}
	
	return 0;
}
