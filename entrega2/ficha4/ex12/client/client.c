/*
 * Renato Oliveira > 1140822@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * client.c
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




#define NUM_SEMS 8

const char *SEM_NAME[NUM_SEMS] = {"sem_request1","sem_request2","sem_request3", "sem_process1","sem_process2","sem_process3","sem_nr_clients","sem_mutex"};
const int NR_SEM_REQUEST1 =0;
const int NR_SEM_REQUEST2 =1;
const int NR_SEM_REQUEST3 =2;
const int NR_SEM_PROCESS1 =3;
const int NR_SEM_PROCESS2 =4;
const int NR_SEM_PROCESS3 =5;
const int SEM_NR_CLIENTS=6;
const int MUTEX=7;


/*
 * PL 4 - Exercise 12
 */
int main(int argc, char *argv[])
{
	
	sem_t *sems[NUM_SEMS];
	// Create mutual exclusion semaphore
	sems[NR_SEM_REQUEST1] = sem_open(SEM_NAME[NR_SEM_REQUEST1], 0); 
	sems[NR_SEM_REQUEST2] = sem_open(SEM_NAME[NR_SEM_REQUEST2], 0); 
	sems[NR_SEM_REQUEST3] = sem_open(SEM_NAME[NR_SEM_REQUEST3], 0); 
	sems[NR_SEM_PROCESS1] = sem_open(SEM_NAME[NR_SEM_PROCESS1], 0); 
	sems[NR_SEM_PROCESS2] = sem_open(SEM_NAME[NR_SEM_PROCESS2], 0); 
	sems[NR_SEM_PROCESS3] = sem_open(SEM_NAME[NR_SEM_PROCESS3], 0); 
	sems[SEM_NR_CLIENTS] = sem_open(SEM_NAME[SEM_NR_CLIENTS], O_CREAT , S_IRUSR|S_IWUSR, 0); 
	sems[MUTEX] = sem_open(SEM_NAME[MUTEX], O_CREAT , S_IRUSR|S_IWUSR, 1); 
	if (sems[NR_SEM_REQUEST1] == SEM_FAILED || sems[NR_SEM_PROCESS1] == SEM_FAILED )
	{
		perror("Semaphore failed.\n");
		exit(EXIT_FAILURE);
	}
	sem_wait(sems[MUTEX]);//make sure every get value is correct, 
	int sval;
	sem_getvalue(sems[SEM_NR_CLIENTS],&sval);
	if(sval>10)
	{
		printf("Max waiting list is 10, you're the %d client waiting so good bye.\n",sval);
	}
	sem_post(sems[SEM_NR_CLIENTS]);
	sem_post(sems[MUTEX]);
	while(1)
	{
		//application logic
		if(sem_trywait(sems[NR_SEM_PROCESS1])==0)//if server 1 is free
		{
			//if entered here, can be removed from waiting queue
			sem_wait(sems[SEM_NR_CLIENTS]);
			sem_post(sems[NR_SEM_REQUEST1]);
			sem_wait(sems[NR_SEM_PROCESS1]);
			printf("I was attended by p1.\n");
			break;
		}
		else
		{
			if(sem_trywait(sems[NR_SEM_PROCESS2])==0)//if server 2 is free
			{
				//if entered here, can be removed from waiting queue
				sem_wait(sems[SEM_NR_CLIENTS]);
				sem_post(sems[NR_SEM_REQUEST2]);
				sem_wait(sems[NR_SEM_PROCESS2]);
				printf("I was attended by p2.\n");
				break;
			}
			else
			{
				if(sem_trywait(sems[NR_SEM_PROCESS3])==0)//if server 3 is free
				{
					//if entered here, can be removed from waiting queue
					sem_wait(sems[SEM_NR_CLIENTS]);
					sem_post(sems[NR_SEM_REQUEST3]);
					sem_wait(sems[NR_SEM_PROCESS3]);
					printf("I was attended by p3.\n");
					break;
				}
			}
		}
   }
	// Close Semaphore
	int i;
	for (i = 0; i < NUM_SEMS; i++) 
	{
		// Close Semaphore
		if (sem_close(sems[i]) < 0)
		{
			perror("SEM close failed.\n");
			exit(EXIT_FAILURE);
		}
		
	}
	
	
	return 0;
}
