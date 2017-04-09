/*
 * Renato Oliveira > 1140822@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * program.c
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <wait.h>

// Setting constants
const int NUMBER_OF_LOOPS = 10000;
const char *SHM_NAME = "/shm_f3_ex03";
/*
 * PL 3 - Exercise 03
 */
int main(void)
{
	// Start variable at 100
	int sh_integer = 100;
	// Vars
	int *sh_int_ptr = &sh_integer;
	int data_size = sizeof(sh_integer), fd;	
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
	sh_int_ptr = (int *) mmap(NULL, data_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if (sh_int_ptr == MAP_FAILED)
	{
		perror("Mapping shared memory failed.");
		exit(EXIT_FAILURE);
	}
	// Create a new process
	pid_t pid;
	pid = fork();
	if(pid < 0)
	{
		perror("Error creating fork.");
		exit(0);
	}
	// Each process does a inc/dec value loop
	int i;
	int counter = 0;
	for (i = 0; i < NUMBER_OF_LOOPS; i++)
	{
		sh_integer++;
		sh_integer--;
		printf("value: %d\n", sh_integer);
		if(sh_integer != 100)
		{
			counter++;
		}
	}
	printf("%s: found %d numbers found different from expected value.\n", (pid == 0) ? "CHILD" : "PARENT", counter);

	if(pid > 0) //parent
	{
		wait(NULL);
		printf("At the end the nuber is %d\n", sh_integer);
		
		// Unmap & close
		if (munmap(sh_int_ptr, data_size) < 0)
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
	}
	
	return 0;
}
