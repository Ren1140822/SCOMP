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



const int BUFFER_SIZE= 81;
const int READ =0;
const int WRITE =1;

// const int PAI_FILHO 0;
// const int FILHO_PAI 1;

/*
 * PL 2 - Exercise 13
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
		close(fd[WRITE]);
	
		 while(read(fd[READ], &buffer, BUFFER_SIZE))
		  {
		 		printf("%s\n",buffer);
		  }
		
		
		// Esperar que o filho acabe (BOA PRÁTICA)
		close(fd[READ]);
		wait(NULL);
		
	} else 				// PROCESSO FILHO
	{
		close(fd[READ]);
		dup2(STDOUT_FILENO,fd[WRITE]);
		execl("/bin/sort","sort","fx.txt",(char*)NULL);
		
		close(fd[WRITE]);
	}
	
	return 0;
}
