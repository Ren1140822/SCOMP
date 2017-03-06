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

#define NUM_CHILDS 2

/*
 * PL 1 - Exercise 2
 */
int main(void)
{

	pid_t pids[NUM_CHILDS];
	int status[NUM_CHILDS];
	int i;

	// Creates n processes
	for (i = 0; i < NUM_CHILDS; i++)
	{
		printf("pid:%d\n", i);
		pids[i] = fork();
		// Check for an error
		if (pids[i] < 0)
		{
			perror("Error while creating child.");
			exit(-1);
		}
		// Only the first parent creates n childs.
		if (pids[i] == 0)
		{
			break;
		}
	}

	// Only first child executes
	if (pids[0] == 0)
	{
		printf("\nPID: %d - I'm the first child.\n", getpid());
		sleep(5);
		exit(1);
	}
	// Only second child executes
	else if (pids[1] == 0)
	{
		printf("\nPID: %d - I'm the second child.\n", getpid());
		exit(2);
	}
	// Only parent executes
	else
	{
		printf("\nPID: %d - I'm the parent.\n", getpid());

		for (i = 0; i < NUM_CHILDS; i++)
		{
			// Waits for his child by order
			waitpid(pids[i], &status[i], 0);
			// Checks if exited with success
			if (WIFEXITED(status[i]))
			{
				printf("The child number %d exited with success. Exit value: %d\n",
						(i + 1), WEXITSTATUS(status[i]));
			} else
			{
				printf("This child failed to exit.\n");
			}
		}
	}

	return 0;
}
