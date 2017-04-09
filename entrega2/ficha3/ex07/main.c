/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
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

#include "circular_buffer.h"

// Setting constants
const int NUM_VALUES = 30;

/*
 * PL 3 - Exercise 07
 */
int main(int argc, char *argv[])
{
	// Shared memory
	circular_buffer *cb;
	int fd, data_size = sizeof(circular_buffer);

	// Open shm
	fd = shm_open(SHM_NAME, O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
	if (fd < 0)
	{
		perror("Open shared memory timed out.");
		exit(EXIT_FAILURE);
	}
	// Truncate shm
	if (ftruncate(fd, data_size) < 0)
	{
		perror("Truncating shared memory failed.");
		exit(EXIT_FAILURE);
	}
	// Map shm
	cb = (circular_buffer *) mmap(NULL, data_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if (cb == MAP_FAILED)
	{
		perror("Mapping shared memory failed.");
		exit(EXIT_FAILURE);
	}
	
	// Initiate circular buffer
	//cb_init(cb);
	
	// Create new process
	pid_t pid;
	pid = fork();
	if (pid > 0) // PRODUCER
	{
		// Write to shm
		int i;
		for (i = 0; i < NUM_VALUES; i++)
		{
			int element = i + 1;
			while (write_to_cb(cb, &element) == NULL); // Writes when space is available
			
			printf("Produced: %d\n", element);
		}
		// Wait for consumer
		wait(NULL);
		// Unmap & close
		if (munmap(cb, data_size) < 0)
		{
			exit(EXIT_FAILURE);
		}
		if (close(fd) < 0)
		{
			exit(EXIT_FAILURE);
		}
		// Unlink
		if (shm_unlink(SHM_NAME) < 0)
		{
			exit(EXIT_FAILURE);
		}	
		printf("SHM Unlinked.\n");	
	} else // CONSUMER
	{
		// Read from shm
		int i;
		for (i = 0; i < NUM_VALUES; i++)
		{
			int element;
			while(read_from_cb(cb, &element) == NULL); // Reads when there is a element to read
			
			printf("Consumed element: %d\n", element);
		}
	}

	return 0;
}
