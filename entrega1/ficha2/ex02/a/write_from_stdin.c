/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * write_from_stdin.c
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "remove_newline.h"


/*
 * Writes all input to pipe until newline ('\n').
 * 
 * returns string's total length.
 */
int write_from_stdin(int fd, char *buffer, int buffer_size)
{
	int end_of_line, tot_length = 0, current_length;
	
	do
	{
		// Get input (fgets reads until buffer_size-1 or \n)
		if (fgets(buffer, buffer_size, stdin) == NULL)
		{
			return -1;
		}
		// Removes ('\n') and verifies if input overflows buffer size
		end_of_line = remove_newline(buffer);
		// Size of string
		current_length = strlen(buffer);
		tot_length += current_length;
		// write to pipe (FIRST: send size -> SECOND: buffer)	
		write(fd, &current_length, sizeof(current_length));
		write(fd, buffer, current_length);
		
	} while (!end_of_line);
	
	return tot_length;
}
