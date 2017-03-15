/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * main.c
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "clear_new_line.h"
#include <string.h>

const int BUFFER_SIZE = 81;
const int READ = 0;
const int WRITE = 1;

/*
 * PL 2 - Exercise 02a
 */
int main(void)
{
	pid_t pid;
	int fd[2];
	int num;
	char buffer[BUFFER_SIZE];
	
	// Criar pipe
	if(pipe(fd) == -1)	// ERRO AO CRIAR PIPE
	{
		perror("pipe failed.\n");
		return -1; 
	}
	
	// Criar novo processo
	pid = fork();
	if (pid < 0)		// ERRO AO CRIAR PROCESSO
	{
		perror("fork failed.\n");
		return -1;
	}
	else if (pid > 0) 	// PROCESSO PAI
	{
		// Fechar descritor de leitura
		close(fd[READ]);
		// Ler inteiro
		printf("PAI: Introduza um inteiro.\n");
		scanf("%d%*c", &num);
		// Escrever inteiro
		write(fd[WRITE], &num, sizeof(num));
		// Ler nome
		printf("PAI: Introduza o seu nome.\n");
		fgets(buffer, BUFFER_SIZE, stdin);
		// Retirar '\n'
		clear_new_line(buffer);
		// Escrever nome
		write(fd[WRITE], buffer, strlen(buffer));
		// Fechar descritor de escrita
		close(fd[WRITE]);
		// Esperar que o filho acabe (BOA PRÁTICA)
		wait(NULL);
	} else 				// PROCESSO FILHO
	{
		// Fechar descritor de escrita
		close(fd[WRITE]);
		// Ler inteiro & nome
		while (read(fd[READ], &num, sizeof(num)) && read(fd[READ], &buffer, BUFFER_SIZE))
		{
			printf("int: %d\nnome: %s\n", num, buffer);
		}
		// Fechar descritor de leitura
		close(fd[READ]);
	}
	
	return 0;
}
