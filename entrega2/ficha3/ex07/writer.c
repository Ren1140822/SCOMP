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


#define VEC_WRITE_SIZE 30
#define VEC_SIZE 10
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
	
	 pid_t pid;
	 pid = fork();
	 if (pid>0)//parent
	 {
		 int i=0;
		 for(i =0;i< VEC_WRITE_SIZE_SIZE;i++)
		 {
			 while(sh_data->writer_index == sh_data->reader_index );
			 sh_data->writer_index = i%VEC_SIZE;
			
			 sh_data->vector[sh_data->writer_index]=i+1;
		 }
	 }
	
	 else{
		  for(i =0;i< VEC_WRITE_SIZE_SIZE;i++)
		  {
			 
			 sh_data->reader = i%VEC_SIZE;
			
			 printf("%d\n",sh_data->vector[sh_data->writer_index]);
		  }
	 }
	 
	
	
	printf("\ndone.\n");
	
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
	
	if (shm_unlink(SHM_NAME) < 0)
	{
		exit(EXIT_FAILURE);
	}
	
	return 0;
}
