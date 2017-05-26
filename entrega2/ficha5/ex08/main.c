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
#include <time.h>
#include <string.h>

// Setting constants
#define NUM_POINTS 16
#define NUM_SNAILS 3
#define BUFF_SIZE 301
// Helper enums
enum racer {snail_a, snail_b, snail_c};
enum point {start_point, finish_point, point_size};
enum grid {snail_num, race_time, grid_size};
// Trail sizes
#define T_SIZE_A 9
#define T_SIZE_B 9
#define T_SIZE_C 8
// Snail struct
typedef struct
{
	int num;
	unsigned int trail_size;
	int (*trail)[point_size];
} snail;
// Global vars
const unsigned int SLIME_MULTIPLIER = 2;
pthread_mutex_t mutexs[NUM_POINTS][NUM_POINTS];
int matrix[NUM_POINTS][NUM_POINTS] = 
{		//	0	1	2	3	4	5	6	7	8	9	10	11	12	13	14	15		
/*0*/	{	-1,	5,	5,	5,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1	},
/*1*/	{	-1,	-1,	-1,	-1,	4,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1	},
/*2*/	{	-1,	-1,	-1,	-1,	4,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1	},
/*3*/	{	-1,	-1,	-1,	-1,	4,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1	},
/*4*/	{	-1,	-1,	-1,	-1,	-1,	3,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1	},
/*5*/	{	-1,	-1,	-1,	-1,	-1,	-1,	5,	5,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1	},
/*6*/	{	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	6,	-1,	-1,	-1,	-1,	-1,	-1,	-1	},
/*7*/	{	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	6,	4,	-1,	-1,	-1,	-1,	-1,	-1	},
/*8*/	{	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	4,	-1,	-1,	-1,	-1,	-1,	-1	},
/*9*/	{	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	5,	-1,	-1,	5,	-1,	-1	},
/*10*/	{	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	5,	5,	-1,	-1,	-1	},
/*11*/	{	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	3	},
/*12*/	{	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	3	},
/*13*/	{	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	5,	-1	},
/*14*/	{	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	3	},
/*15*/	{	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1	}
};

// Error constants
const int THREAD_CREATE_ERROR = -1;
const int THREAD_JOIN_ERROR = -2;
const int THREAD_EXIT_ERROR = -3;
const int MUTEX_INIT_ERROR = -4;
const int MUTEX_DESTROY_ERROR = -5;
const int INVALID_TRAIL_ERROR = -6;
// Thread function
void *race(void *param);
void order_grid(unsigned int grid[NUM_SNAILS][grid_size]);

/*
 * PL 5 - Exercise 08
 */
int main(void)
{
	// Set trails
	snail snails[NUM_SNAILS];
	// Snail A
	int trail_a[T_SIZE_A][point_size] = {{0,1}, {1,4}, {4,5}, {5,6}, {6,8}, {8,9}, {9,10}, {10,11}, {11,15}}; /* { start, end } */
	snails[snail_a].trail_size = T_SIZE_A;
	snails[snail_a].trail = trail_a;
	// Snail B
	int trail_b[T_SIZE_B][point_size] = {{0,2}, {2,4}, {4,5}, {5,7}, {7,8}, {8,9}, {9,13}, {13,14}, {14,15}}; /* { start, end } */
	snails[snail_b].trail_size = T_SIZE_B;
	snails[snail_b].trail = trail_b;
	// Snail C
	int trail_c[T_SIZE_C][point_size] = {{0,3}, {3,4}, {4,5}, {5,7}, {7,9}, {9,10}, {10,12}, {12,15}}; /* { start, end } */
	snails[snail_c].trail_size = T_SIZE_C;
	snails[snail_c].trail = trail_c;
	// Init mutexs
	int i, j;
	for (i = 0; i < NUM_POINTS; i++)
	{
		for (j = 0; j < NUM_POINTS; j++)
		{
			if (matrix[i][j] > 0) // Checks if its a valid trail
			{
				if (pthread_mutex_init(&mutexs[i][j], NULL) != 0) 
				{
					perror("Init mutex failed.\n");
					exit(MUTEX_INIT_ERROR);
				}
			}
		}
	}
	// Create Threads
	pthread_t threads[NUM_SNAILS];
    for (i = 0; i < NUM_SNAILS; i++)
    {
		snails[i].num = i + 1;
		if (pthread_create(&threads[i], NULL, race, (void *) &snails[i]) != 0) 
		{
			perror("Create thread failed.\n");
			exit(THREAD_CREATE_ERROR);
		}
	}
	// Joins with each terminated thread
	unsigned int classifications[NUM_SNAILS][grid_size]; // { snail_num, time }
	for (i = 0; i < NUM_SNAILS; i++)
	{
		classifications[i][snail_num] = i + 1; // Get snail num
		unsigned int *ret;
		if(pthread_join(threads[i], (void *)&ret) != 0)
		{
			perror("Join thread failed.\n");
			exit(THREAD_JOIN_ERROR);
		}
		classifications[i][race_time] = *ret;
		free(ret);
	}
	// CLASSIFICATION
	order_grid(classifications);
	puts("\n### SNAIL RACE ###\n");
	puts("Position | SNAIL # | TIME");
	unsigned int position = 1, prev = 1;
	for (i = 0; i < NUM_SNAILS; i++)
	{
		if (i > 0 && (classifications[i-1][race_time] == classifications[i][race_time]))
		{
			printf("%8u | %7u | %4u\n", prev, classifications[i][snail_num], classifications[i][race_time]);
		} 
		else
		{
			printf("%8u | %7u | %4u\n", position, classifications[i][snail_num], classifications[i][race_time]);
			prev = position;
		}
		position++;
	}
	
	return 0;
}

// thread function
void *race(void *param)
{
	// Convert param
	snail info = *((snail *) param);
	// Start race
	time_t start, finish;
	unsigned int *elapsed_time = (unsigned int *) malloc(sizeof(unsigned int));
	*elapsed_time = 0;
	start = time(NULL);
	// Race log
	char log[BUFF_SIZE] = "";
	// Race
	int i;
	for (i = 0; i < info.trail_size; i++)
	{
		// Get coordinates
		int x = info.trail[i][start_point];
		int y = info.trail[i][finish_point];
		if (matrix[x][y] < 0)
		{ // if invalid trail point returns -1
			perror("Invalid trail.");
			exit(INVALID_TRAIL_ERROR);
		}
		pthread_mutex_lock(&mutexs[x][y]); // Locks trail
		char tmp[BUFF_SIZE];
		snprintf(tmp, BUFF_SIZE, "[%d->%d:%ds]", x, y, matrix[x][y]);
		strncat(log, tmp, BUFF_SIZE);	
		sleep(matrix[x][y]); // trail duration
		matrix[x][y] *= SLIME_MULTIPLIER; // MULTIPLY the duration due to the slime (default x2)
		pthread_mutex_unlock(&mutexs[x][y]); // Unlocks trail
	}
	// Finish race
	finish = time(NULL);
	*elapsed_time = (unsigned int) difftime(finish,start);
	printf("[SNAIL #%d] race time : %u seconds <%s>\n",info.num, *elapsed_time, log);
	
	pthread_exit((void *) elapsed_time);
}

void order_grid(unsigned int grid[NUM_SNAILS][grid_size])
{
	for (int i = 1; i < NUM_SNAILS; i++)
	{
		int current_snail = grid[i][snail_num];
		int current_time = grid[i][race_time]; 
		int j = i;
		while (j > 0 && grid[j-1][race_time] > current_time)
		{
			grid[j][0] = grid[j-1][snail_num];
			grid[j][1] = grid[j-1][race_time];
			j--;
		}
		grid[j][snail_num] = current_snail;
		grid[j][race_time] = current_time;
	}
}
	












