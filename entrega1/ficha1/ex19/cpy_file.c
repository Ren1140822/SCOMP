/*
 * Renato Oliveira > 1140822@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * cpy_file.c
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define CPY_FOLDER "backup"
#define BIN_PATH "/bin/"
#define CPY_CMD "cp"

/*
 * copies a file to the backup folder
 * 
 */
int copy_file(char* file_name)
{
	pid_t pid = fork();
	if(pid<0)
	{
		perror("Error creating fork.");
	}
	if (pid==0)
	{
		int ret;		
		char * first_command=BIN_PATH;
		//concatenaçóes dos caminhos e comandos
		char * complete_command = (char*)malloc(strlen(first_command)+strlen(file_name)+strlen(CPY_FOLDER));
		strcpy(complete_command,first_command);
		strcat(complete_command,CPY_CMD);
		
		//estrutura fica: /bin/cp, cp , nome ficheiro, nome pasta
		ret = execl(complete_command,CPY_CMD,file_name,CPY_FOLDER,(char*)NULL);	
		exit(ret);
	}
	wait(NULL);
	return 0;
}
