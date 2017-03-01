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

/*
 * Module 1 - Exercise 2
 */	
int main(void) {
	
	pid_t pid_a, pid_b;
	int status_a, status_b;
	
	// Creates first process
	pid_a = fork();
	
	// Check for an error
	if (pid_a < 0)
	{
		perror("Error while creating process a.");
		exit(-1);
	}
	
	if (pid_a > 0) // Only parent executes
	{
		printf("\nPID: %d - I'm the parent.\n", getpid());
		
		// Creates second process
		pid_b = fork();
		
		// Check for an error
		if (pid_b < 0)
		{
			perror("Error while creating process b.");
			exit(-1);
		}
		
		if (pid_b > 0) // Only parent executes
		{
			// Waits for first child
			waitpid(pid_a, &status_a, 0);
			// Checks if exited with success
			if (WIFEXITED(status_a))
			{
				printf("The first child exited with success. Exit value: %d\n", WEXITSTATUS(status_a));
			} else
			{
				printf("The first child failed to exit.\n");
			}
			
			// Waits for second child
			waitpid(pid_b, &status_b, 0);
			// Checks if exited with success
			if (WIFEXITED(status_b))
			{
				printf("The second child exited with success. Exit value: %d\n", WEXITSTATUS(status_b));
			} else
			{
				printf("The second child failed to exit.\n");
			}
			
		} else // Only second child executes
		{
			printf("\nPID: %d - I'm the second child.\n", getpid());
			exit(2);
		}
	} else // Only first child executes
	{
		printf("\nPID: %d - I'm the first child.\n", getpid());
		sleep(5);
		exit(1);
	}
	
	return 0;
}
