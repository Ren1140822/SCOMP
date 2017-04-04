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
#include <string.h>

#include "create_childs.h"
#include "shd_type.h"
#include "shm_helper.h"
#include "word_search.h"

// Setting constants
#define NUM_CHILDS 10
const char *TXT_FOLDER = "txt/";
const char *WORDS[NUM_CHILDS] = {"hello", "bye", "scomp", "isep", "portugal", "hello", "bye", "scomp", "isep", "portugal"};
const int WRITE_TIMEOUT = 500;
const int MSG_SIZE = 101;

/*
 * PL 3 - Exercise 06
 */
int main(int argc, char *argv[])
{
	// Shared memory
	shd_type *sh_data;
	int fd, data_size = sizeof(shd_type) * NUM_CHILDS;

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
	sh_data = (shd_type *) mmap(NULL, data_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if (sh_data == MAP_FAILED)
	{
		perror("Mapping shared memory failed.");
		exit(EXIT_FAILURE);
	}
	
	// Create processes
	int seq;
	seq = create_childs(NUM_CHILDS);
	if (seq == 0) // PARENT
	{
		// Write info in shm
		int i;
		for (i = 0; i < NUM_CHILDS; i++)
		{
			if (snprintf(sh_data[i].path, BUFFER_SIZE, "%stxt%d.txt", TXT_FOLDER, (i + 1)) < 0)
			{
				perror("path too long.");
				exit(EXIT_FAILURE);
			}
			if (strncpy(sh_data[i].word, WORDS[i], BUFFER_SIZE) == NULL)
			{
				perror("something went wrong.");
				exit(EXIT_FAILURE);
			}
			// Set flag
			sh_data[i].rd_flag = 1;
		}
		
		// Wait for children to complete tasks
		while (wait(NULL) > 0);
		
		// Print occurences
		for (i = 0; i < NUM_CHILDS; i++)
		{
			if (sh_data[i].occurrences == -1)
			{
				char msg[MSG_SIZE];
				snprintf(msg, MSG_SIZE, "The file:%stxt%d.txt did not open.", TXT_FOLDER, (i + 1));  
				perror(msg);
				
			} else
			{
				printf("Child #%d:\tfound %d occurences of the word: %s (file path: %s)\n", 
				(i + 1), sh_data[i].occurrences, sh_data[i].word, sh_data[i].path);
			}
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
		
		// Unlink
		if (shm_unlink(SHM_NAME) < 0)
		{
			exit(EXIT_FAILURE);
		}		
	} else // CHILDREN
	{
		// Wait until father to finish writing
		if (wait_for_write(&(sh_data[seq - 1].rd_flag), WRITE_TIMEOUT) < 0)
		{
			perror("Waiting for father timed out.");
			exit(EXIT_FAILURE);
		}
		
		//Open file and search
		sh_data[seq - 1].occurrences = number_of_occurences(sh_data[seq - 1].path, sh_data[seq - 1].word);
	}

	return 0;
}
