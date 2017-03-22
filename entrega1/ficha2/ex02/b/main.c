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

#define BUFFER_SIZE 81
typedef struct
{
	int num;
	char nome[BUFFER_SIZE];
} input;

const int READ = 0;
const int WRITE = 1;

/*
 * PL 2 - Exercise 02b
 */
int main(void)
{
	// vars
	pid_t pid;
	int fd[2];
	input in;

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
		// Ler numero & nome
		scanf("%d%*c", &in.num);
		fgets((char *)&in.nome, BUFFER_SIZE, stdin);
		// Escrever no struct
		write(fd[WRITE], &in, sizeof(in));
		// Fecha descritor de escrita
		close(fd[WRITE]);
		// Esperar que o filho acabe (BOA PRÁTICA)
		wait(NULL);
	} else 				// PROCESSO FILHO
	{
		// Fechar descritor de escrita
		close(fd[WRITE]);
		// Ler inteiro & nome
		printf("PROCESSO-FILHO>\n");
		// Lê o numero introduzido
		read(fd[READ], &in, sizeof(in));
		printf("int: %d\nnome: %s", in.num, in.nome);
		// Fechar descritor de leitura
		close(fd[READ]);
	}
	
	return 0;
}
