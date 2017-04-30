/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * exhibitor.c
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

#include "sem_array.h"

// Setting constants
#define SEMS_SIZE 4
const int NUM_SEATS = 5;

const int OPEN_DOORS = 0; // Barrier
const int SEATS = 1; // Limited resources
const int START = 2; // Barrier
const int END = 3; // Barrier

const int TIME_TO_START = 2; // Time doors are opened
const int BREAK_TIME = 5; // Time between sessions
const int EXHBITION_DURATION = 10; // Duration of the exhibition

const int NUM_SESSIONS = 3; // Number of sessions of the exhibition

/*
 * PL 4 - Exercise 14 - EXHIBITOR
 */
int main(int argc, char *argv[])
{
	
	// Open semaphores
	sem_t *sems[SEMS_SIZE];
	int sem_values[SEMS_SIZE] = { 0 /* OPEN_DOORS */, NUM_SEATS /* SEATS */, 0 /* START */, 1 /* END */ };
	if (create_sem_array(sems, SEMS_SIZE, O_CREAT | O_EXCL, sem_values) == NULL)
	{
		perror("Semaphores failed.\n");
		exit(EXIT_FAILURE);
	}
	
	/***********************************************/
	int counter = 1;
	do
	{
		/********* OPEN DOORS **********/
		
		sem_post(sems[OPEN_DOORS]); // Open doors
		printf("<EXHIBITOR> : Doors opened.\n");
		sleep(TIME_TO_START); // Keeps doors open for a certain period
		printf("<EXHIBITOR> : Doors closed.\n");
		
		/********* OPEN DOORS **********/
		
		sem_wait(sems[OPEN_DOORS]); // Close doors
		
		/********* START EXHIBITION **********/
		
		sem_wait(sems[END]); // Lock end of previous exhibition
		sem_post(sems[START]); // Start exhibtion
		
		printf("<EXHIBITOR> : Exhibition has started (Session #%d).\n", counter);
		
		/********* START EXHIBITION **********/
		
		sleep(EXHBITION_DURATION); // Time of exhibition
		
		/********* END EXHIBITION **********/
		
		sem_wait(sems[START]); // Lock start of next exhibition
		sem_post(sems[END]); // End exhibtion
		
		printf("<EXHIBITOR> : Exhibition has ended (Session #%d).\n", counter);
		
		/********* END EXHIBITION **********/
		
		/********* TIME BETWEEN EXHIBITIONS **********/
		
		sleep(BREAK_TIME);
		
		/********* TIME BETWEEN EXHIBITIONS **********/
		
		counter++;
	} while (counter <= NUM_SESSIONS);
	/***********************************************/
	
	// Close Semaphores
	if (close_sem_array(sems, SEMS_SIZE) < 0)
	{
		perror("SEMs close failed.\n");
		exit(EXIT_FAILURE);
	}
	// Unlink Semaphores
	if (unlink_sem_array(sems, SEMS_SIZE) < 0)
	{
		perror("SEMs unlink failed.\n");
		exit(EXIT_FAILURE);
	}
	
	return 0;
}
