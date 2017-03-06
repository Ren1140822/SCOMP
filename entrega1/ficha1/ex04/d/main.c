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

/*
 * PL 1 - Exercise 4c
 */
int main(void)
{
	pid_t pid;
	int f;
	
	for (f = 0; f < 3; f++)
	{
		pid = fork();
		if (pid > 0)
		{
			printf("Eu sou o PAI\n");
		}
		else
		{
			// Sempre que um filho seja criado é forcado a sair do ciclo.
			break;
		}
	}

	return 0;
}
