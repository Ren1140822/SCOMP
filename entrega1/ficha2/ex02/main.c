/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
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
#include "doe.h"

const int BUFFER_SIZE 81;
const int READ 0;
const int WRITE 1;
// const int PAI_FILHO 0;
// const int FILHO_PAI 1;

/*
 * PL 2 - Exercise 00
 */
int main(void)
{
	pid_t pid;
	int fd[2]; 			// dois pipes: fd[2][2]
	
	char buffer[BUFFER_SIZE];
	
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
		close(fd[READ]);
		
		/* Working with strings
		 * snprintf(buffer, BUFFER_SIZE, "%d", getpid());
		 * write(fd[WRITE], buffer, strlen(buffer));
		 */
		
		
		// write(fd[WRITE], &msg, size);
		
		close(fd[WRITE]);
		
		// Esperar que o filho acabe (BOA PRÁTICA)
		wait(NULL);
		
	} else 				// PROCESSO FILHO
	{
		close(fd[WRITE]);
		
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
		
		close(fd[READ]);
	}
	
	return 0;
}
