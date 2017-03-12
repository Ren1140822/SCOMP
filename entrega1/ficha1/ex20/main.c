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

#define CAP 25

/*
 * PL 1 - Exercise 20
 */
int main(void)
{

	int balance = CAP, ronda = 1, aposta = 0, num_escolhido = 0, num_gerado = 0;
	
	char *batota = "exec";
	char *path = "batota/exec";

	printf("\n\n## APOSTA QUE GANHA OU NÃO ##\n\n\n");

	// Loop do jogo
	do
	{
		printf("<> Ronda %d <>\n\n\nO seu saldo: %d EUROS.\n\nQuanto quer apostar?\n", ronda, balance);
		scanf("%d", &aposta);
		while (aposta < 1 || aposta > balance)
		{
			printf("Quantia inválida.\n");
			scanf("%d", &aposta);
		}
		
		printf("Escolha um número de 1 a 5.\n");
		scanf("%d", &num_escolhido);
		while (num_escolhido < 1 || num_escolhido > 5)
		{
			printf("Número inválida.\n");
			scanf("%d", &num_escolhido);
		}
		
		// Create process to execute batota
		pid_t pid = fork();
		if (pid == 0)
		{
			int ret;
			// exec()
			ret = execl(path, batota, (char *) NULL);
			// Terminate child
			exit(ret);
		}
		int status;
		wait(&status);
		if (WIFEXITED(status))
		{
			num_gerado = WEXITSTATUS(status);
			
			if (num_gerado > 0 && num_gerado < 6)
			{
				printf("E o número mágico é .");
				int i;
				for (i = 0; i < 3; i++)
				{
					fflush(stdout);
					sleep(1);
					printf(" .");
				}
				printf(" %d !\n\n", num_gerado);
				
				if (num_escolhido == num_gerado)
				{
					balance += (aposta * 2);
					
					printf("Parabéns!\n\n");
				} else
				{
					balance -= aposta;
					printf("Tente a sorte novamente!\n\n");
				}
			} else 
			{
				printf("Failed!\n");
				exit(-1);
			}
		}
		ronda++;
	} while(balance > 1);
	
	printf("\n## GAMEOVER ##.\n");
	
	return 0;
}
