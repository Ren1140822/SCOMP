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
#include <string.h>
#include "create_childs.h"

// Descriptor constants
const int READ = 0;
const int WRITE = 1;

// Settings constants
const int NUM_CHILDS = 2;
const char *PATH = "./";
const int CMD_SIZE = 51;
const int MIN_ARGS = 4;

/*
 * PL 2 - Exercise 16
 */
int main(int argc,char *argv[])
{
	// check args
	if (argc < MIN_ARGS)
	{
		perror("insuficiente arguments.\n");
		return EXIT_FAILURE;
	}
	// vars
	int fd[2], seq;
	// Create pipe
	if(pipe(fd) < 0)
	{
		perror("pipe failed.\n");
		return -1; 
	}
	// Create child
	seq = create_childs(NUM_CHILDS);
	if (seq < 0)
	{
		perror("fork failed.\n");
		return -1; 
	}
	else if (seq == 1)
	{
		// Close read descriptor
		close(fd[READ]);
		//Redirect stdout to pipe's write descriptor
		if(dup2(fd[WRITE], STDOUT_FILENO) < 0)
		{
			perror("dup failed.");
			exit(EXIT_FAILURE);
		}
		// Close wite descriptor
		close(fd[WRITE]);
		// First Command received
		char cmd[CMD_SIZE];
		strcpy(cmd, PATH);
		strcat(cmd, argv[1]);
		// Exec
		int ret;
		ret = execlp(cmd, cmd, (char *) NULL);
		// If exec fails
		exit(ret);
	}
	else if (seq == 2)
	{
		// Close read descriptor
		close(fd[WRITE]);
		//Redirect stdout to pipe's write descriptor
		if(dup2(fd[READ], STDIN_FILENO) < 0)
		{
			perror("dup failed.");
			exit(EXIT_FAILURE);
		}
		// Close wite descriptor
		close(fd[READ]);
		// Open file and get descriptor
		FILE *ptr_file;
		ptr_file = fopen(argv[3], "w");
		if( ptr_file == NULL ) 
		{
			perror("open file failed.\n");
			exit(EXIT_FAILURE);
		}
		int file_d = fileno(ptr_file);
		// Redirect stdout to file
		dup2(file_d, STDOUT_FILENO);
		close(file_d);
		// First Command received
		char cmd[CMD_SIZE];
		strcpy(cmd, PATH);
		strcat(cmd, argv[2]);
		// Exec
		int ret;
		ret = execlp(cmd, cmd, (char *) NULL);
		// If exec fails
		exit(ret);
	}
	// Close Pipe (PARENT)
	close(fd[READ]);
	close(fd[WRITE]);
	// Wait for childs
	printf("loading . . .\n");
	while(wait(NULL) > 0);
	printf("\nfinished! please check file %s for result\nTip: use command: cat %s\n", argv[3], argv[3]);
	
	return 0;
}
