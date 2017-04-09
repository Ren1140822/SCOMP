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
#include <string.h>
#include <time.h>
#include <sys/wait.h>
#include "shd_type.h"




/*
 * PL 3 - Exercise 05 
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
	if(pid>0)//parent
	{
		printf("Insert student number.\n");
		scanf("%d%*c",&sh_data->numero);
		printf("Insert student name.\n");
		char name[STR_SIZE];
		fgets(name,STR_SIZE,stdin);
		strncpy(sh_data->nome,name,STR_SIZE);
		int i;
		for(i=0;i<NR_DISC;i++)
		{
			printf("Insert grade of class %d.\n",i+1);
			scanf("%d",&sh_data->disciplinas[i]);
		}
		sh_data->w_flag = 1;
		wait(NULL);
	}
	else
	{
		//waiting for write
		while(sh_data->w_flag==0);
		printf("==========================================\n");
		//reading structure data.
		printf("Student number is: %d.\n",sh_data->numero);
		printf("Student name is: %s\n",sh_data->nome);
		int i;
		for(i=0;i<NR_DISC;i++)
		{
			printf("Grade of class %d is: %d.\n",i+1,sh_data->disciplinas[i]);
			
		}
		exit(0);
	}
	
	
	
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
