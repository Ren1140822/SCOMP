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


const int READ = 0;
const int WRITE = 1;
// const int PAI_FILHO 0;
// const int FILHO_PAI 1;

/*
 * PL 2 - Exercise 01
 */
int main(void)
{
	pid_t pid;
	int fd[2]; 			// dois pipes: fd[2][2]
	int msg;
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
		int msg=getpid();
		printf("Sou o pai com o pid:%d\n",msg);
		write(fd[WRITE], &msg, sizeof(pid_t));
		close(fd[WRITE]);
		// Esperar que o filho acabe (BOA PRÁTICA)
		wait(NULL);
		
	} else 				// PROCESSO FILHO
	{
		close(fd[WRITE]);
		while(read(fd[READ], &msg, sizeof(pid_t)))
	    {
		   printf("Pid do meu pai:%d\n",msg);
		}
		close(fd[READ]);
	}
	
	return 0;
}
