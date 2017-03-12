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

#define SIZE 3

/*
 * PL 1 - Exercise 4h
 */
int main(void)
{
	pid_t pid[SIZE];
	int f, status;
	
	for (f = 0; f < SIZE; f++)
	{
		pid[f] = fork();
		if (pid[f] < 0)
		{
			perror("Erro ao criar filho.\n");
			exit(-1);
		}
		if (pid[f] > 0)
		{
			printf("Eu sou o PAI\n");
		}
		else
		{
			sleep(3);
			// Numero de ordem (f + 1).
			exit(f+1);
		}
	}
	
	printf("loading ");
	// Waits for child
	while (waitpid(pid[1], &status, WNOHANG) != pid[1])
	{
		printf(". ");
		fflush(stdout);
		sleep(1);
	}
	
	// Checks if exited with success
	if (WIFEXITED(status))
	{
		printf("\nO filho na ordem #%d e pid #%d terminou com exito.\n", WEXITSTATUS(status), pid[1]);
	}

	return 0;
}
