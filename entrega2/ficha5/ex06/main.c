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
#define NUM_LINES 3
#define NUM_JOURNEYS 2
#define JOURNEY_LENGTH 2
#define BUFF_SIZE 161
enum line { a_b, b_c, b_d }; // Railway lines
enum train {train_a, train_b}; // Trains

// Train info struct
typedef struct
{
	char origin[BUFF_SIZE];
	char destination[BUFF_SIZE];
	unsigned int train_number;
	enum line *journey;
} train_info;

#define MAX_NUM 3
#define MIN_NUM 1

// Error constants
const int THREAD_CREATE_ERROR = -1;
const int THREAD_JOIN_ERROR = -2;
const int THREAD_EXIT_ERROR = -3;
const int MUTEX_INIT_ERROR = -4;
const int MUTEX_DESTROY_ERROR = -5;

// Global vars
pthread_mutex_t mutexs[NUM_LINES];

// Thread function
void *travel(void *param);
void line_description(enum line num, char *desc);
void hour(char *hour);

/*
 * PL 5 - Exercise 06
 */
int main(void)
{
	// Set journeys
	train_info journeys[NUM_JOURNEYS];
	// Train A
	strncpy(journeys[train_a].origin, "cityA", BUFF_SIZE);
	strncpy(journeys[train_a].destination, "cityD", BUFF_SIZE);
	enum line journey_a[JOURNEY_LENGTH] = {a_b, b_d};
	journeys[train_a].journey = journey_a; // A->B->D
	// Train B
	strncpy(journeys[train_b].origin, "cityC", BUFF_SIZE);
	strncpy(journeys[train_b].destination, "cityD", BUFF_SIZE);
	enum line journey_b[JOURNEY_LENGTH] = {b_c, b_d};
	journeys[train_b].journey = journey_b; // C->B->D
	// Init mutexs
	int i;
	for (i = 0; i < NUM_LINES; i++)
	{
		if (pthread_mutex_init(&mutexs[i], NULL) != 0) 
		{
			perror("Init mutex failed.\n");
			exit(MUTEX_INIT_ERROR);
		}
	}
	// Create Threads
	pthread_t threads[NUM_JOURNEYS];
    for (i = 0; i < NUM_JOURNEYS; i++)
    {
		journeys[i].train_number = i + 1; // Assign train number
		if (pthread_create(&threads[i], NULL, travel, (void *) &journeys[i]) != 0) 
		{
			perror("Create thread failed.\n");
			exit(THREAD_CREATE_ERROR);
		}
	}
	// Joins with each terminated thread
	char *journey_string;
	for (i = 0; i < NUM_JOURNEYS; i++)
	{
		if(pthread_join(threads[i], (void *)&journey_string) != 0)
		{
			perror("Join thread failed.\n");
			exit(THREAD_JOIN_ERROR);
		}
		// Print arrival message
		puts(journey_string);
	}
	return 0;
}

// thread function
void *travel(void *param)
{
	// Intializes random seed
	srand((unsigned) pthread_self());
	// Convert param
	train_info info = *((train_info *) param);
	// Return allocation
	char *time_string = (char *) malloc(BUFF_SIZE * sizeof(char));
	// Save departure time
	char start_time[BUFF_SIZE];
	hour(start_time);
	// Complete return string
	snprintf(time_string, BUFF_SIZE, "[Train #%02d] >> TIME: [%s] -> [", 
		info.train_number, start_time);
	// Journey iteration
	int i;
	for (i = 0; i < JOURNEY_LENGTH; i++)
	{
		enum line current_line = info.journey[i];
		
		pthread_mutex_lock(&mutexs[current_line]); // Locks intersection
		char line_string[BUFF_SIZE];
		line_description(current_line, line_string);
		// Print Station Info
		printf("[Train #%02d] [Origin: %s | Destination: %s] [LINE: %s]\n", 
		info.train_number, info.origin, info.destination, line_string);
		// Simulate journey duration
		sleep((rand() % MAX_NUM) + MIN_NUM);
		// sleep(3); // [TESTING] If mutex is working one journey should 3 more seconds
		pthread_mutex_unlock(&mutexs[current_line]); // Unlocks intersection
	}
	// Save arrival time
	char end_time[BUFF_SIZE];
	hour(end_time);
	strcat(time_string, end_time);
	strcat(time_string, "]");
	// exit with departure/arrival time
	pthread_exit((void*) time_string);
}

void hour(char *hour)
{
	// Get seconds since the Epoch
	time_t current_time;
	time(&current_time);
	// Convert to localtime
	struct tm *local = localtime(&current_time);
	// Build the string
	strftime(hour,BUFF_SIZE,"%H:%M:%S", local);
}

void line_description(enum line num, char *desc)
{
	switch (num)
	{
		case a_b:
			strncpy(desc, "A<->B", BUFF_SIZE);
			break;
		case b_c:
			strncpy(desc, "B<->C", BUFF_SIZE);
			break;
		case b_d:
			strncpy(desc, "B<->D", BUFF_SIZE);
			break;
		default:
			strncpy(desc, "off rail", BUFF_SIZE);
	}
}
	












