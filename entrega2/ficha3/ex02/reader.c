/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * reader.c
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */

#include <string.h>
#include "shd_type.h"
#include "shm_helper.h"
#include "calc.h"

// Setting constants
const int OPEN_TIMEOUT = 120;
const int WRITE_TIMEOUT = 500;

/*
 * PL 3 - Exercise 02 - READER
 */
int main(int argc, char *argv[])
{
	// Vars
	int fd, data_size = sizeof(shd_type);
	shd_type *sh_data;
	// Wait until writer finishes writing
	printf("Waiting for writer to open shared memory . . .\n");
	// Open shm
	fd = shm_open_rd_mode(SHM_NAME, OPEN_TIMEOUT);
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
	sh_data = (shd_type *) mmap(NULL, data_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if (sh_data == MAP_FAILED)
	{
		perror("Mapping shared memory failed.");
		exit(EXIT_FAILURE);
	}
	
	// Wait until writer finishes writing
	printf("[ok]\n\nWaiting for writer to finish writing . . .\n");
	if (wait_to_write(&sh_data->w_flag, WRITE_TIMEOUT) < 0)
	{
		perror("Waiting for writer timed out.");
		exit(EXIT_FAILURE);
	}
	
	
	// Prints vector's average
	printf("[ok]\n\nThe average is : %.2f.\n", average(sh_data->vector, SHD_VEC_SIZE));
	
	
	// Unmap & close
	if (munmap(sh_data, data_size) < 0)
	{
		exit(EXIT_FAILURE);
	}
	if (close(fd) < 0)
	{
		exit(EXIT_FAILURE);
	}
	// Unlink option
	if ((argc > 1) && (strcmp("-rm", argv[1]) == 0))
	{
		if (shm_unlink(SHM_NAME) < 0)
		{
			exit(EXIT_FAILURE);
		}
	}
	return 0;
}
