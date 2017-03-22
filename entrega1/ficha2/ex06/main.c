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
#include "vector_helper.h"

const int READ = 0;
const int WRITE = 1;
const int VEC_SIZE = 10;
const int NUM_CHILDS = 5;
/*
 * PL 2 - Exercise 06
 */
int main(void)
{
	// vars
	pid_t pids[NUM_CHILDS];
	int seq;
	int vec1[VEC_SIZE], vec2[VEC_SIZE], results[VEC_SIZE];
	int fd[NUM_CHILDS][2];
	// Fill vectors
	fill_vector(vec1, VEC_SIZE);
	fill_vector(vec2, VEC_SIZE);
	// Print Arrays
	fputs("Vec1:", stdout);
	print_vec(vec1, VEC_SIZE);
	fputs("Vec2:", stdout);
	print_vec(vec2, VEC_SIZE);
	// Create Pipes
	int i;
	for (i = 0; i < NUM_CHILDS; i++)
	{
		// Create each pipe & check for errors
		if(pipe(fd[i]) < 0)
		{
			perror("pipe failed.\n");
			return -1; 
		}
	}
	// Create childs
	seq = create_childs(pids, NUM_CHILDS);
	if (seq < 0)
	{
		perror("fork failed.\n");
		return -1; 
	}
	if (seq > 0) // CHILDS
	{
		// Temp var
		int temp;
		// Pipe ID
		int pipe_id = (seq - 1);
		// Magnitude each processo should take care of
		int magnitude = VEC_SIZE / NUM_CHILDS;
		// Close read descriptor
		close(fd[pipe_id][READ]);
		// Write file to pipe
		for(i = magnitude * (seq - 1); i < magnitude * seq; i++)
		{
			temp = vec1[i] + vec2[i];
			write(fd[pipe_id][WRITE], &temp, sizeof(temp));
		}
		// Close write descriptor
		close(fd[pipe_id][WRITE]);
		exit(EXIT_SUCCESS);
	}
	// PARENT
	int j = 0; // results iterator index
	for (i = 0; i < NUM_CHILDS; i++)
	{
		// WAIT
		waitpid(pids[i], NULL, 0);
		// Close write descriptor
		close(fd[i][WRITE]);
		// Reads pipe content
		while (read(fd[i][READ], (results + j), sizeof(int)) && j < VEC_SIZE)
		{
			j++;
		}
		// Close read descriptor
		close(fd[i][READ]);
	}
	
	// Print Results
	fputs("\nRESULTS:", stdout);
	print_vec(results, VEC_SIZE);
		
	return 0;
}
