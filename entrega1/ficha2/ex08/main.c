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
#include "sale.h"
#include "create_childs.h"
#include "vector_helper.h"

const char *separator = "\n+==============================================================+\n";
const int READ = 0;
const int WRITE = 1;
const int VEC_SIZE = 50000;
const int NUM_CHILDS = 10;

/*
 * PL 2 - Exercise 08
 */
int main(void)
{
	// vars
	int seq;
	sale sales[VEC_SIZE];
	int fd[2];
	// Fill vectors
	fill_sales(sales, VEC_SIZE);
	// Print Arrays
	fputs("sales:", stdout);
	print_sales(sales, VEC_SIZE);
	// Create each pipe & check for errors
	if(pipe(fd) < 0)
	{
		perror("pipe failed.\n");
		return -1; 
	}
	// Create childs
	seq = create_childs(NUM_CHILDS);
	if (seq < 0)
	{
		perror("fork failed.\n");
		return -1; 
	}
	if (seq > 0) // CHILDS
	{
		// vars
		int i;
		// Magnitude each processo should take care of
		int magnitude = VEC_SIZE / NUM_CHILDS;
		// Close read descriptor
		close(fd[READ]);
		// Write file to pipe
		for(i = magnitude * (seq - 1); i < magnitude * seq; i++)
		{
			if (sales[i].quantity > 20)
			{
				write(fd[WRITE], &sales[i], sizeof(sale));
			}
		}
		// Close write descriptor
		close(fd[WRITE]);
		exit(EXIT_SUCCESS);
	}
	// PARENT
	// Create vector
	sale *ptr_bigger = (sale *) malloc(sizeof(sale));
	if (ptr_bigger == NULL)
	{
		perror("Malloc failed.");
		return EXIT_FAILURE;
	}
	// Close write descriptor
	close(fd[WRITE]);
	// Reads pipe content
	int i = 0;
	while (read(fd[READ], (ptr_bigger + i), sizeof(sale)))
	{
		i++;
		ptr_bigger = (sale *) realloc(ptr_bigger, (i + 1) * sizeof(sale));
		if (!ptr_bigger)
		{
			perror("Realloc failed.");
			return EXIT_FAILURE;
		}
	}
	// Close read descriptor
	close(fd[READ]);
	// wait for childs to terminate (BOAS PRÁTICAS)
	while (wait(NULL) > 0);
	// Print Results
	fputs(separator, stdout);
	fputs("\nRESULTS:", stdout);
	print_sales(ptr_bigger, i);
	// Free dyanmic vector
	free(ptr_bigger);
		
	return 0;
}
