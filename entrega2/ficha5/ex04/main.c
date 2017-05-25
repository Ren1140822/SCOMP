/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
 * Renato Oliveira > 1140822@isep.ipp.pt
 * 
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
#include <pthread.h>

// Setting constants
#define DB_SIZE 10000
#define KEY_SIZE 5
#define NUM_THREADS 10
#define MAX_NUM 49
#define MIN_NUM 1

// Error constants
const int THREAD_CREATE_ERROR = -1;
const int THREAD_JOIN_ERROR = -2;
const int THREAD_EXIT_ERROR = -3;
const int MUTEX_INIT_ERROR = -4;
const int MUTEX_DESTROY_ERROR = -5;

// Global vars
int db[DB_SIZE][KEY_SIZE];
int num_count[MAX_NUM];
pthread_mutex_t mutexs[MAX_NUM];

// Thread function
void *statistics(void *param);

/*
 * PL 5 - Exercise 04
 */
int main(void)
{
	/* Intializes random seed */
	srand((unsigned) getpid());
	// Fill db with random keys 
	int i, j, k;
	for (i = 0; i <= DB_SIZE; i++)
	{
		for (j = 0; j < KEY_SIZE; j++)
		{
			db[i][j] = (rand() % MAX_NUM) + MIN_NUM;
			// Verify if key already contains the assigned number 
			// (if true assign new number)
			for (k = 0; k < j; k++)
			{
				if (db[i][k] == db[i][j])
				{
					j--; // reset to the same index
				}
			}
		}
	}
	// Init mutexs
	for (i = 0; i < MAX_NUM; i++)
	{
		if (pthread_mutex_init(&mutexs[i], NULL) != 0) 
		{
			perror("Init mutex failed.\n");
			exit(MUTEX_INIT_ERROR);
		}
	}
	// Create Threads
	pthread_t threads[NUM_THREADS];
	int thread_index[NUM_THREADS]; // each thread's index
    for (i = 0; i < NUM_THREADS; i++)
    {
		thread_index[i] = i + 1;
		if (pthread_create(&threads[i], NULL, statistics, (void *) &thread_index[i]) != 0) 
		{
			perror("Create thread failed.\n");
			exit(THREAD_CREATE_ERROR);
		}
	}
	// Joins with each terminated thread
	void *status;
	for (i = 0; i < NUM_THREADS; i++)
	{
		if(pthread_join(threads[i], &status) != 0)
		{
			perror("Join thread failed.\n");
			exit(THREAD_JOIN_ERROR);
		}
		if (status != NULL)
		{
			perror("thread didn't exit normally");
			exit(THREAD_EXIT_ERROR);
		}
	}
	// Print results
	printf ("The lotary statistics results:\n\n");
	for (i = 0; i < MAX_NUM; i++)
	{
		printf("The number [ #%2d ] appeared %d time(s).\n", i + 1, num_count[i]);
		if (pthread_mutex_destroy(&mutexs[i]) != 0)
		{
			perror("Destroy mutex failed.\n");
			exit(MUTEX_DESTROY_ERROR);
		}
	}
	
	return 0;
}

// thread function
void *statistics(void *param)
{
	int index = *((int *) param);
	
	int i, j, magnitude = DB_SIZE / NUM_THREADS;
	for (i = ((index - 1) * magnitude); i < (index * magnitude); i++)
	{
		for (j = 0; j < KEY_SIZE; j++)
		{
			int number = db[i][j] - 1; // index 0 -> number 1
			// "fecha" a variável mutex do number em questão.
			// Se já estiver "fechada" espera até
			// que seja possível fechar.
			pthread_mutex_lock(&mutexs[number]);
			num_count[number]++; // incrementa o count do numero
			// Volta a "abrir" a variável mutex.
			pthread_mutex_unlock(&mutexs[number]);
		}
	}
	pthread_exit(NULL);
}
