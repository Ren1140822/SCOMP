/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * parking_lot.c
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

/*
 * PL 4 - Exercise 16 - PARKING_LOT
 */
int main(int argc, char *argv[])
{
	// Open semaphores
	sem_t *sems[SEMS_ORDER_SIZE + NUM_QUEUES + SEMS_PS_SIZE];
	int sem_values[SEMS_ORDER_SIZE + NUM_QUEUES + SEMS_PS_SIZE] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, PARK_SPACE };
	if (create_sem_array(sems, (SEMS_ORDER_SIZE + NUM_QUEUES + SEMS_PS_SIZE), O_CREAT|O_EXCL, sem_values) == NULL)
	{
		perror("Semaphore failed.\n");
		exit(EXIT_FAILURE);
	}

	// Shared memory
	shm_type *shm; 
	int fd, data_size = sizeof(shm_type);
	// Open shm
	fd = shm_open(SHM_NAME, O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
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
	
	/******************************************************************/
	printf("Parking Lot OPENED.\n");
	sem_wait(sems[REQUEST_PS]); // Wait for priority system request
	
	do {
		
		sem_wait(sems[PARK_FULL]); // Wait for free a space
		int i;
		for (i = 0; i < NUM_QUEUES; i++)
		{
			
			int queue_no = QUEUE_TEACHERS + i;
			if(sem_trywait(sems[queue_no]) == 0)
			{
				int release_no = RELEASE_TEACHER + i;
				sem_post(sems[release_no]);
				break; // Skip & start again
			}
		}
		
		
	} while(1);
	/******************************************************************/
	printf("Parking Lot CLOSED.\n");
	// Close Semaphores
	if (close_sem_array(sems, (SEMS_ORDER_SIZE + NUM_QUEUES + SEMS_PS_SIZE)) < 0)
	{
		perror("SEMs close failed.\n");
		exit(EXIT_FAILURE);
	}
	// Unlink Semaphores
	if (unlink_sem_array(sems, (SEMS_ORDER_SIZE + NUM_QUEUES + SEMS_PS_SIZE)) < 0)
	{
		perror("SEM unlink failed.\n");
		exit(EXIT_FAILURE);
	}
	printf("\nSEMs unlinked.\n");
	
	// Unmap & close SHM
	if (munmap(shm, data_size) < 0)
	{
		exit(EXIT_FAILURE);
	}
	if (close(fd) < 0)
	{
		exit(EXIT_FAILURE);
	}
	// Unlink SHM
	if (shm_unlink(SHM_NAME) < 0)
	{
		exit(EXIT_FAILURE);
	}	
	printf("SHM Unlinked.\n");
	
	return 0;
}
