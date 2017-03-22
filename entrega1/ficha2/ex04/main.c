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
#include "custom_read.h"

const int READ = 0;
const int WRITE = 1;
const char *FILENAME = "file.txt";
const int BUFFER_SIZE = 81;
/*
 * PL 2 - Exercise 04
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
	seq = create_childs(1);
	if (seq < 0)
	{
		perror("fork failed.\n");
		return -1; 
	}
	else if (seq == 0) // FATHER
	{
		// Close read descriptor
		close(fd[READ]);
		// Write file to pipe
		if (write_to_pipe(fd[1], FILENAME, BUFFER_SIZE) < 0)
		{
			perror("write to pipe failed.\n");
			return -1; 
		}
		// Close write descriptor
		close(fd[WRITE]);
		// Wait for child
		wait(NULL);
	}
	else if (seq == 1) // CHILD
	{
		// Close write descriptor
		close(fd[WRITE]);
		
		// Reads pipe content
		fputs("CHILD>\nContent of File\n::\n", stdout);
		read_to_stdout(fd[READ], BUFFER_SIZE);
		fputs("::\nEOF\n<CHILD\n", stdout);
		// Close read descriptor
		close(fd[READ]);
	}
	return 0;
}
