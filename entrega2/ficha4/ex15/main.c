/*
 * Renato Oliveira > 1140822@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * main.c
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <semaphore.h> 

#include <string.h>

#define NUM_SEMS 3

const char *SEM_NAME[NUM_SEMS] = {"sem_east_west","sem_west_east","sem_mutex"};
const int EAST_WEST =0;
const int WEST_EAST =1;
const int CROSS_TIME=10;
const int MUTEX=2;
const int SLEEP_TIME=3;
const int LOOPS_NUMBER = 100;



/*
 * PL 4 - Exercise 15
 */
int main(int argc, char *argv[])
{
	// Create mutual exclusion semaphore

	
	sem_t *sems[NUM_SEMS];
	
	
	sems[MUTEX] =  sem_open(SEM_NAME[MUTEX],  O_CREAT , S_IRUSR|S_IWUSR, 1); 
	sems[EAST_WEST] = sem_open(SEM_NAME[EAST_WEST],  O_CREAT , S_IRUSR|S_IWUSR, 0); 
	sems[WEST_EAST] = sem_open(SEM_NAME[WEST_EAST],  O_CREAT, S_IRUSR|S_IWUSR, 0); 
	int i;
	for (i = 0; i < NUM_SEMS; i++) 
	{
		if (sems[i] == SEM_FAILED)
		{
			perror("Semaphore failed.\n");
			exit(EXIT_FAILURE);
		}
	}
	
	//application logic
	printf("Press 1 to cross from east to west, 2 to cross from west to east.\n");
	int option,sval;
	scanf("%d",&option);
	int nr;
	for (nr = 0; nr < LOOPS_NUMBER; nr++) //main loop
	{			
		if(option==1)
		{
			sem_wait(sems[MUTEX]);//mutex to guarantee value is real - one process might process this line milliseconds before the other process posts the east_west sempahore, so it should be invalid
			sem_getvalue(sems[WEST_EAST],&sval);//if no cars are crossing in the other direction this sempahore value is 0
			if(sval==0)
			{
				sem_post(sems[EAST_WEST]);//if 0 we increase our direction's semaphore so cars in opposite direction cant cross
				sem_post(sems[MUTEX]);
				printf("Crossing east to west.\n");
				sleep(CROSS_TIME);
				sem_wait(sems[EAST_WEST]);//after we finish crossing, we decrease the semaphore's value by 1
				break;
			}
			else
			{
				sem_post(sems[MUTEX]);
				printf("A car is crossing in the opposite direction.\n");
				sleep(SLEEP_TIME);//not to syncronize anything, just to slow the message spam!!!
			}
		}
		else
		{
			if(option==2)
			{
				sem_wait(sems[MUTEX]);
				sem_getvalue(sems[EAST_WEST],&sval);//if no cars are crossing in the other direction this sempahore value is 0
				if(sval==0)
				{
					sem_post(sems[WEST_EAST]);
					sem_post(sems[MUTEX]);
					printf("Crossing west to east.\n");
					sleep(CROSS_TIME);
					sem_wait(sems[WEST_EAST]);
					break;
				}
				else
				{
					sem_post(sems[MUTEX]);
					printf("A car is crossing in the opposite direction.\n");
					sleep(SLEEP_TIME);//not to syncronize anything, just to slow the message spam!!!
				}
			}
			else{
				printf("Invalid option selected. Exiting.\n");
				break;
			}
		}
	}
	
	
	// Close Semaphore
	
	for (i = 0; i < NUM_SEMS; i++) // Close & unlink both SEM
	{
		// Close Semaphore
		if (sem_close(sems[i]) < 0)
		{
			perror("SEM close failed.\n");
			exit(EXIT_FAILURE);
		}
			// Unlink option
		if ((argc > 1) && (strcmp("-rm", argv[1]) == 0))
		{
			// Unlink Semaphore
			if (sem_unlink(SEM_NAME[i]) < 0)
			{
				perror("SEM unlink failed.\n");
				exit(EXIT_FAILURE);
			}
		}
	}
	printf("\nSEMs unlinked & closed with success.\n");
	
	return 0;
}
