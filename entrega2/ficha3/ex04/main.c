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

#include "vector_helper.h"
#include "create_childs.h"

// SHM
#define SH_VEC_SIZE 10
const char *SHM_NAME = "/shm_f3_ex04";

// Setting constants
const unsigned int VEC_SIZE = 100;
const int R_MIN = 0;
const int R_MAX = 100;
const unsigned int NUM_CHILDS = 10;

/*
 * PL 3 - Exercise 04
 */
int main(int argc, char *argv[])
{
	// Intial vector
	int vec[VEC_SIZE];
	fill_with_random_ints(vec, VEC_SIZE, R_MIN, R_MAX);
	// Print vector that will be searched
	print_vec(vec, VEC_SIZE);
	
	// Shared memory vector
	int *sh_vector;
	int fd, data_size = sizeof(int) * SH_VEC_SIZE;
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
	sh_vector = (int *) mmap(NULL, data_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if (sh_vector == MAP_FAILED)
	{
		perror("Mapping shared memory failed.");
		exit(EXIT_FAILURE);
	}
	
	
	// Create processes
	int seq;
	seq = create_childs(NUM_CHILDS);
	
	if (seq == 0) // PARENT
	{
		while (wait(NULL) > 0);
		
		puts("\nSHD_VECTOR:");
		print_vec(sh_vector, SH_VEC_SIZE);
		
		// Find max
		int maximum = find_max(sh_vector, SH_VEC_SIZE);
		
		printf("\nThe maximum value found is: %d\n", maximum);
		
		// Unmap & close
		if (munmap(sh_vector, data_size) < 0)
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
	} else
	{
		unsigned int magnitude = VEC_SIZE / NUM_CHILDS;
		// find max in their search magnitude
		int *search_vec = vec + ((seq - 1) * magnitude);
		int maximum = find_max(search_vec, magnitude);
		// write to shared vector the maximum found
		sh_vector[seq - 1] = maximum;
		printf("**(Child #%d) max value found: %d**\n", seq, maximum);
	}

	return 0;
}
