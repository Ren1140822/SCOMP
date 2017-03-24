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
#include "custom_write.h"

// Descriptor constants
const int READ = 0;
const int WRITE = 1;

// Settings constants
const int NUM_CHILDS = 1;
const char *CMD = "more";
const int NUM_LINES = 100;

/*
 * PL 2 - Exercise 14
 */
int main(void)
{
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
	else if (seq == 0) // FATHER
	{
		
		// Close read descriptor
		close(fd[READ]);
		// Send lines
		write_to_pipe(fd[WRITE], NUM_LINES);
		// Close write descriptor
		close(fd[WRITE]);
		// Wait for child
		wait(NULL);
	}
	else if (seq == 1) // CHILD
	{
		printf("Child process executing bash cmd: %s\n\n", CMD);
		// Close pie's write descriptor
		close(fd[WRITE]);
		// Redirect stdin to pipe
		dup2(fd[READ], STDIN_FILENO);
		// Close pipe's read descriptor
		close(fd[READ]);
		// Execute command
		execlp(CMD, CMD,(char*)NULL);
		// If exec fails
		perror("exec error.");
		exit(EXIT_FAILURE);
	}
	
	return 0;
}
