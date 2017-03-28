/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * writer.c
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */

#include <time.h>
#include "shd_type.h"
#include "vector_helper.h"

// Setting constants
const int MIN_VALUE = 1;
const int MAX_VALUE = 20;

/*
 * PL 3 - Exercise 02 - WRITER
 */
int main(void)
{
	// Vars
	int fd, data_size = sizeof(shd_type);
	shd_type *sh_data;
	// Open shm
	fd = shm_open(SHM_NAME, O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
	if (fd < 0)
	{
		perror("Error opening shared memory.");
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
	
	// Initialize random
	time_t t;
	srand((unsigned) time(&t));
	// Write to vector
	int i;
	for (i = 0; i < SHD_VEC_SIZE; i++)
	{
		sh_data->vector[i] = rand() % MAX_VALUE + MIN_VALUE;
	}
	// Set w_flag
	sh_data->w_flag = 1;
	
	print_vec(sh_data->vector, SHD_VEC_SIZE);
	printf("\ndone.\n");
	
	return 0;
}
