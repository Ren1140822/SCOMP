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
#include "str_split.h"

#define BUFFER_SIZE 10000 

/*
 * PL 1 - Exercise 16
 */
int main(void)
{
	// command line input buffer 
	char buffer[BUFFER_SIZE];
	// cmd to exit my shell
	char *quit = "sair\n";
	// Iterate until exit cmd is entered
	do
	{
		// Read
		printf("my_shell: ");
		fgets(buffer, BUFFER_SIZE, stdin);
		// if exit cmd terminate
		if (!strcmp(buffer, quit))
		{
			break;
		}
		// Create process to execute command
		pid_t pid = fork();
		if (pid == 0)
		{
			// split cmds 
			char **cmd = str_split(buffer, " \n");
			// exec()
			execvp(cmd[0], cmd);
			// Terminate child
			exit(-1);
		}
		int status;
		wait(&status);
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		{
			printf("Illegal cmd: %s", buffer);
		}
		
		
	} while(1);
	
	printf("\nSession terminated.\n");
	
	return 0;
}
