/*
 * Renato Oliveira > 1140822@isep.ipp.pt
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
#include <wait.h>


// Setting constants
const int NUMBER_OF_LOOPS=10000000;
/*
 * PL 3 - Exercise 00 - WRITER
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
	
	/*
	 * 
	 * application code for writer
	 * 
	 */
	 
	 
	 sh_data->number=100;


	
	
	pid_t pid;
	pid = fork();
	if(pid<0)
	{
		perror("Error creating fork.");
		exit(0);
	}
	if(pid>0){//parent
		int i;
		int counter=0;

		for (i=0;i<NUMBER_OF_LOOPS;i++)
		{
			sh_data->number++;
			sh_data->number--;
			if(sh_data->number!=100)
			{
				counter ++;
				
			}
			
		}
		wait(NULL);
		printf("PARENT found %d numbers found different from expected value.\n",counter);
	}
	else
	{
		int i;
		int counter=0;

		for (i=0;i<NUMBER_OF_LOOPS;i++)
		{
			sh_data->number++;
			sh_data->number--;
			if(sh_data->number!=100)
			{
				counter ++;
			}
			
		}
		printf("SON found %d  numbers found different from expected value.\n",counter);
	}
	return 0;
}
