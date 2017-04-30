/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * visitor.c
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

/*
 * PL 4 - Exercise 14 - VISITOR
 */
int main(int argc, char *argv[])
{
	
	// Open semaphores
	sem_t *sems[SEMS_SIZE];
	int sem_values[SEMS_SIZE] = { 0 /* OPEN_DOORS */, NUM_SEATS /* SEATS */, 0 /* START */, 1 /* END */ };
	if (create_sem_array(sems, SEMS_SIZE, O_EXCL, sem_values) == NULL)
	{
		perror("Semaphores failed.\n");
		exit(EXIT_FAILURE);
	}

	/***********************************************/
	sem_wait(sems[OPEN_DOORS]); sem_post(sems[OPEN_DOORS]); // Block when doors are closed
	
	sem_wait(sems[SEATS]); // Block when no more seats
	
	/********* SEATED **********/
	
	printf("<VISITOR> : I'm seated.\n"); // Simulate visitior siting
	
	/********* SEATED **********/
	
	sem_wait(sems[START]); sem_post(sems[START]); // Unlock when exhibition has started
	/********* IN THE EXHIBITION **********/
	
	printf("<VISITOR> : I'm watching the exhibition.\n"); // During the exhibition
	
	/********* IN THE EXHIBITION **********/
	
	sem_wait(sems[END]); sem_post(sems[END]); // Unlock when exhibition has ended
	
	sem_post(sems[SEATS]); // Release seat
	
	/********* LEAVING EXHIBITION **********/
	
	printf("<VISITOR> : I'm leaving the exhibition.\n"); // During the exhibition
	
	/********* LEAVING EXHIBITION **********/
	
	/***********************************************/
	// Close Semaphores
	if (close_sem_array(sems, SEMS_SIZE) < 0)
	{
		perror("SEMs close failed.\n");
		exit(EXIT_FAILURE);
	}
	
	return 0;
}
