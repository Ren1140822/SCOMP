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
#include "create_childs.h"

// Constants
const int READ = 0;
const int WRITE = 1;
const int NUM_CHILDS = 5;
const int NUM_PIPES = 6;
const int MAX_VALUE = 100;

/*
 * PL 2 - Exercise 10
 */
int main(void)
{
	// vars
	int seq, i;
	int fd[NUM_PIPES][2];
	// Create each pipe & check for errors
	for (i = 0; i < NUM_PIPES; i++)
	{
		if(pipe(fd[i]) < 0)
		{
			perror("pipe failed.\n");
			return -1; 
		}
	}
	// Create childs
	seq = create_childs(NUM_CHILDS);
	if (seq < 0)
	{
		perror("fork failed.\n");
		return -1; 
	}
	
//	   0
//	 /   \
//	1     5
//	|	  |
//	2	  4
//	 \   /
//	   3 
	// ALL PROCESSES
	// Match each pipe's end to a process
	int read_pipe = (seq == 0) ? 5 : (seq - 1);
	int write_pipe = seq;
	// All processes should close write descriptor on their reading pipe
	close(fd[read_pipe][WRITE]);
	// All processes should close read descriptor on their writing pipe
	close(fd[write_pipe][READ]);
	// Intializes random number generator
	pid_t pid = getpid();
	srand((unsigned) pid);
	int rand_num = rand() % MAX_VALUE;
	// Print PID & Generated Number
	printf("Seq: %d\tPID: %d\tGenerated Number: %d\n", seq, pid, rand_num);
	
	// Flow biggest number through pipes until parent process.
	// CHILDS
	if (seq > 0)
	{
		// Receives a number from reading pipe
		int received_num;
		read(fd[read_pipe][READ], &received_num, sizeof(received_num));
		close(fd[read_pipe][READ]);
		// Checks biggest number & sends it through writing pipe
		received_num = (received_num >= rand_num) ? received_num : rand_num;
		write(fd[write_pipe][WRITE], &received_num, sizeof(received_num));
		close(fd[write_pipe][WRITE]);
		// Exit
		exit(EXIT_SUCCESS);
	}
	// PARENT ONLY
	// Parent passes his number to the next process
	write(fd[write_pipe][WRITE], &rand_num, sizeof(rand_num));
	close(fd[write_pipe][WRITE]);
	// Reads the biggest number
	int received_num;
	read(fd[read_pipe][READ], &received_num, sizeof(received_num));
	close(fd[read_pipe][READ]);
	// Prints the biggest number
	printf("+==========================================+");
	printf("\n\nThe WINNER is: %d.\n", received_num);
	
	// Wait for child processes
	while(wait(NULL) > 0);
		
	return 0;
}










