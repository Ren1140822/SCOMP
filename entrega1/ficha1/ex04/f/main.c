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
 * PL 1 - Exercise 4f
 */
int main(void)
{
	pid_t pid;
	int f;

	for (f = 0; f < 3; f++)
	{
		pid = fork();
		if (pid < 0)
		{
			perror("Erro ao criar filho.");
			exit(-1);
		}

		if (pid > 0)
		{
			printf("Eu sou o PAI\n");
		}
		else
		{
			sleep(1);
			// Sempre que um filho seja criado é forcado a sair do ciclo.
			break;
		}
	}

	if (pid > 0)
	{
		// Espera que cada filho termine sem ordem (a função wait devolve o pid do filho).
		while(wait(NULL) > 0);
	}
	else
	{
		exit(1);
	}

	return 0;
}
