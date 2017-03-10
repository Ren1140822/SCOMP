/*
 * Renato Oliveira > 1140822@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * system_clone.c
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

/*
 * 
 * 
 */
int system_clone(char* command)
{
	pid_t pid=fork();
	if(pid<0)
	{
		perror("Error creating fork.");
	}
	if (pid==0)
	{
		int ret;
		
		char * bin_command="/bin/";
		char * complete_command = (char*)malloc(strlen(bin_command)+strlen(command));
		strcpy(complete_command,bin_command);
		strcat(complete_command,command);
		
		ret = execl(complete_command,command,(char*)NULL);
		
		exit(ret);
	}
	wait(NULL);
	return 0;
}
