/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * car.c
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

#include "sem_array.h"
#include "shm_type.h"

// Setting constants
#define SEMS_ORDER_SIZE 4
#define NUM_QUEUES 3
#define SEMS_PS_SIZE 5

const int SHM = 0; 				// semaphore to access SHM (mutual exclusion)

const int ORDER_BARRIER1 = 1; 	// semaphore as a barrier so that each car can check if 
								// he is next in line (mutual exclusion)
							 
const int ORDER_BARRIER2 = 2;	// semaphore as a barrier so that only when all processes that 
								// have a waiting ticket pass the first barrier. This is to avoid the
								// possibility of one process controlling the barrier and entering a deadlock (by occurence of events)

const int ORDER_QUEUE = 3; 		// semaphore to control a order queue (by occurence of events)

const int QUEUE_TEACHERS = 4; 	// semaphore to control a queue of teachers (by occurence of events)
const int QUEUE_CLERKS = 5; 	// semaphore to control a queue of clerks (by occurence of events)
const int QUEUE_STUDENTS = 6; 	// semaphore to control a queue of students (by occurence of events)

const int REQUEST_PS = 7; 		// semaphore to request parking lot to enable priority system (by occurence of events)
const int RELEASE_TEACHER = 8; 	// semaphore to release a teacher (by occurence of events)
const int RELEASE_CLERK = 9; 	// semaphore to release a clerk (by occurence of events)
const int RELEASE_STUDENT = 10; // semaphore to release a student (by occurence of events)
const int PARK_FULL = 11; 		// semaphore to control park space (limited resources)

const int PARK_SPACE = 2; 		// number of available park spaces

const int MIN_WAIT = 1;
const int MAX_WAIT = 10;

const char *CLERK_CMD = "-c";
const char *STUDENT_CMD = "-s";

/*
 * PL 4 - Exercise 16 - CAR
 */
int main(int argc, char *argv[])
{
	// Check type of car (teacher, clerk or student)
	int queue_no = QUEUE_TEACHERS;
	int release_no = RELEASE_TEACHER;
	if ( (argc > 1))
	{
		if (strcmp(STUDENT_CMD, argv[1]) == 0)
		{
			queue_no = QUEUE_STUDENTS;
			release_no = RELEASE_STUDENT;
			
		} else if (strcmp(CLERK_CMD, argv[1]) == 0)
		{
			queue_no = QUEUE_CLERKS;
			release_no = RELEASE_CLERK;
		}		
		// ELSE DEFAULT
	}
	
	// Open semaphores
	int sem_values[SEMS_ORDER_SIZE + NUM_QUEUES + SEMS_PS_SIZE] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, PARK_SPACE };
	sem_t *sems[SEMS_ORDER_SIZE + NUM_QUEUES + SEMS_PS_SIZE];
	if (create_sem_array(sems, (SEMS_ORDER_SIZE + NUM_QUEUES + SEMS_PS_SIZE), O_EXCL, sem_values) == NULL)
	{
		perror("Semaphore failed.\n");
		exit(EXIT_FAILURE);
	}
	
	// Shared memory
	shm_type *shm; 
	int fd, data_size = sizeof(shm_type);
	// Open shm
	fd = shm_open(SHM_NAME, O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
	if (fd < 0)
	{
		perror("Open shared memory failed.");
		exit(EXIT_FAILURE);
	}
	// Truncate shm
	if (ftruncate(fd, data_size) < 0)
	{
		perror("Truncating shared memory failed.");
		exit(EXIT_FAILURE);
	}
	// Map shm
	shm = (shm_type *) mmap(NULL, data_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if (shm == MAP_FAILED)
	{
		perror("Mapping shared memory failed.");
		exit(EXIT_FAILURE);
	}
	
	/* Intializes random sleep time */
	srand((unsigned) getpid());
	unsigned int parked_time = (rand() % MAX_WAIT) + MIN_WAIT;
	
	/******************************************************************/
	printf("Arrived.\n");
	// Force order
	int order_no;
	sem_wait(sems[SHM]); // Wait for exclusive access to shm
	order_no = shm->order_no;
	shm->order_no++; // next order number
	sem_post(sems[SHM]); // Unlock shm
	
	// Waiting mechanism
	unsigned int pass = 0;
	while (!pass)
	{
		sem_wait(sems[ORDER_BARRIER1]); sem_post(sems[ORDER_BARRIER1]); // first barrier to block cars in queue
		
		if (order_no == shm->next) // Check if it's his turn
		{
			pass = 1;
		}
		
		// to pass first barrier and then close barrier 1 & open barrier 2
		/**************************************************************/
		sem_wait(sems[SHM]); // Wait for exclusive access to shm
		/***************** CRITICAL SECTION ***************************/
		sem_post(sems[ORDER_QUEUE]); // Increment queue
		
		int num_cars; 
		sem_getvalue(sems[ORDER_QUEUE], &num_cars); // Get number of cars in queue
		
		if ( num_cars == (shm->order_no - shm->next) ) // Check if last car is between barriers
		{
			sem_wait(sems[ORDER_QUEUE]); // Decrement by 1 queue
			sem_wait(sems[ORDER_BARRIER1]); // Close barrier 1
			sem_post(sems[ORDER_BARRIER2]); // Open barrier 2
		}
		/***************** CRITICAL SECTION ***************************/
		sem_post(sems[SHM]); // Unlock shm
		/**************************************************************/
		
		sem_wait(sems[ORDER_BARRIER2]); sem_post(sems[ORDER_BARRIER2]); // second barrier blocks waiting cars before proceeding
		/** This barrier prevents that 1 process can enter an infinite loop by being always the 
		 * first to pass the barrier 1 and not being the next that should pass**/
		
		// Decrement queue size
		if (sem_trywait(sems[ORDER_QUEUE]) < 0) // Last client passing the barrier 2 closes barrier 2
		{
			sem_wait(sems[ORDER_BARRIER2]); // Close barrier 2
		}
	}

	(shm->next)++; // Next order number

	sem_wait(sems[SHM]); // Wait for exclusive access to shm
	/******************** CRITICAL SECTION ************************/
	if (!shm->full)
	{
		/*********************** CHECK IF FULL ****************************/
		if (sem_trywait(sems[PARK_FULL]) < 0)
		{
			//sem_wait(sems[SHM]); // Wait for exclusive access to shm
			/******************** CRITICAL SECTION ************************/
			shm->full = 1; // Park is full
			/******************** CRITICAL SECTION ************************/
			//sem_post(sems[SHM]); // Unlock shm
		}
		/*********************** CHECK IF FULL ****************************/
		sem_post(sems[ORDER_BARRIER1]); // Free next in line
	}
	
	
	
	if (shm->full) // FULL
	{
		/******************** CRITICAL SECTION ************************/
		sem_post(sems[SHM]); // Unlock shm
		
		/************************** PRIORITY **************************/
		sem_post(sems[REQUEST_PS]); // Send request for priority system (2nd criteria)
		
		sem_post(sems[queue_no]); // Queue for specific type (teacher, clerk & student)
		
		sem_post(sems[ORDER_BARRIER1]); // Free next in line
		
		sem_wait(sems[release_no]); // Wait for parking lot authorization
		/************************** PRIORITY **************************/
		
	} else // NOT FULL
	{
		/******************** CRITICAL SECTION ************************/
		sem_post(sems[SHM]); // Unlock shm
	}
	
	/**************************** PARKING *****************************/
	printf("Parked.\n");
	sleep(parked_time); // Time parked
	sem_post(sems[PARK_FULL]); // Release a parking space
	printf("Left.\n");
	/**************************** PARKING *****************************/
	/******************************************************************/
	// Close Semaphores
	if (close_sem_array(sems, (SEMS_ORDER_SIZE + NUM_QUEUES + SEMS_PS_SIZE)) < 0)
	{
		perror("SEMs close failed.\n");
		exit(EXIT_FAILURE);
	}
	
	// Unmap & close SHM
	if (munmap(shm, data_size) < 0)
	{
		exit(EXIT_FAILURE);
	}
	if (close(fd) < 0)
	{
		exit(EXIT_FAILURE);
	}
	
	return 0;
}
