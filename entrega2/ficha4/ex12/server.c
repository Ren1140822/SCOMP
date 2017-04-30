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
#include "create_childs.h"
#include "generate_random.h"

#define NUM_SEMS 7

const char *SEM_NAME[NUM_SEMS] = {"sem_request1","sem_request2","sem_request3", "sem_process1","sem_process2","sem_process3","sem_mutex"};
const int NR_SEM_REQUEST1 =0;
const int NR_SEM_REQUEST2 =1;
const int NR_SEM_REQUEST3 =2;
const int NR_SEM_PROCESS1 =3;
const int NR_SEM_PROCESS2 =4;
const int NR_SEM_PROCESS3 =5;
const int MUTEX =6;
const int LOOPS_NUMBER = 20;

/*
 * PL 4 - Exercise 12
 */
int main(int argc, char *argv[])
{
	
	sem_t *sems[NUM_SEMS];
	// Create mutual exclusion semaphore
	sems[NR_SEM_REQUEST1] = sem_open(SEM_NAME[NR_SEM_REQUEST1], O_CREAT , S_IRUSR|S_IWUSR, 0); 
	sems[NR_SEM_REQUEST2] = sem_open(SEM_NAME[NR_SEM_REQUEST2], O_CREAT , S_IRUSR|S_IWUSR, 0); 
	sems[NR_SEM_REQUEST3] = sem_open(SEM_NAME[NR_SEM_REQUEST3], O_CREAT , S_IRUSR|S_IWUSR, 0); 
	sems[NR_SEM_PROCESS1] = sem_open(SEM_NAME[NR_SEM_PROCESS1], O_CREAT , S_IRUSR|S_IWUSR, 0); 
	sems[NR_SEM_PROCESS2] = sem_open(SEM_NAME[NR_SEM_PROCESS2],O_CREAT , S_IRUSR|S_IWUSR, 0); 
	sems[NR_SEM_PROCESS3] = sem_open(SEM_NAME[NR_SEM_PROCESS3], O_CREAT , S_IRUSR|S_IWUSR, 0); 

	if (sems[NR_SEM_REQUEST1] == SEM_FAILED || sems[NR_SEM_PROCESS1] == SEM_FAILED)
	{
		perror("Semaphore failed.\n");
		exit(EXIT_FAILURE);
	}
	int seq;
	seq = create_childs(2);
	int nr;
	for (nr = 0; nr < LOOPS_NUMBER; nr++) 
	{
		//application logic
		if(seq==0)
		{
			printf("p1\n");
			sem_post(sems[NR_SEM_PROCESS1]);
			sem_wait(sems[NR_SEM_REQUEST1]);//if no client wakes him up, he stays blocked
			int wait_time = generate_random(1,10);
			printf("Attending client...\n");
			sleep(wait_time);//taking care of client
			sem_post(sems[NR_SEM_PROCESS1]);
		}
		if(seq==1)
		{
			printf("p2\n");
			sem_post(sems[NR_SEM_PROCESS2]);
			sem_wait(sems[NR_SEM_REQUEST2]);
			int wait_time = generate_random(1,10);
			printf("Attending client...\n");
			sleep(wait_time);//taking care of client
			sem_post(sems[NR_SEM_PROCESS2]);
		}
		if(seq==2)
		{
			printf("p3\n");
			sem_post(sems[NR_SEM_PROCESS3]);
			sem_wait(sems[NR_SEM_REQUEST3]);
			int wait_time = generate_random(1,10);
			printf("Attending client...\n");
			sleep(wait_time);//taking care of client
			sem_post(sems[NR_SEM_PROCESS3]);
		}	
	}
	
	// Close Semaphore
	int i;
	for (i = 0; i < NUM_SEMS; i++) // Close & unlink both SEM
	{
		// Close Semaphore
		if (sem_close(sems[i]) < 0)
		{
			perror("SEM close failed.\n");
			exit(EXIT_FAILURE);
		}
		// Unlink Semaphore
		if (sem_unlink(SEM_NAME[i]) < 0)
		{
			perror("SEM unlink failed.\n");
			exit(EXIT_FAILURE);
		}
	}
	printf("\nSEMs unlinked & closed with success.\n");
	
	return 0;
}
