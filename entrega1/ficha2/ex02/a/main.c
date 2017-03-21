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
#include <string.h>
#include "write_from_stdin.h"
#include "read_to_stdout.h"

const int BUFFER_SIZE = 3;
const int READ = 0;
const int WRITE = 1;

/*
 * PL 2 - Exercise 02a
 */
int main(void)
{
	// vars
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
		
		printf("PROCESSO-PAI> Introduza um inteiro e depois uma string.\n");
		// Ler inteiro
		scanf("%d%*c", &num);
		// Escrever inteiro
		write(fd[WRITE], &num, sizeof(num));
		// Ler & escrever nome
		if (write_from_stdin(fd[WRITE], buffer, BUFFER_SIZE) < 0)
		{
			perror("write failed.");
			return -1;
		}
		// Fecha descritor de escrita
		close(fd[WRITE]);
		// Esperar que o filho acabe (BOA PRÁTICA)
		wait(NULL);
	} else 				// PROCESSO FILHO
	{
		// Fechar descritor de escrita
		close(fd[WRITE]);
		// Ler inteiro
		read(fd[READ], &num, sizeof(num));
		printf("PROCESSO-FILHO> int: %d\n", num);
		// Ler nome
		printf("PROCESSO-FILHO> nome: ");
		read_to_stdout(fd[READ], buffer, BUFFER_SIZE);
		// Fechar descritor de leitura
		close(fd[READ]);
	}
	
	return 0;
}
