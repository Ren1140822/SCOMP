/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * custom_write.c
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/*
 * Write to pipe content of a file.
 * 
 * returns the file length.
 */
int write_to_pipe(int fd, const char *filename, const int buffer_size)
{
	// vars
	int length = 0, current_length;
	// Intialize buffer
	char buffer[buffer_size];
	// Open txt file
	FILE *ptr_file = fopen(filename, "r");
	// if open was unsuccessful
	if (!ptr_file)
	{
		return -1;
	}
	// Iterate file lines until EOF
	while (fgets(buffer, buffer_size, ptr_file))
	{
		// Update length
		current_length = strlen(buffer);
		length += current_length;
		// write to pipe (FIRST: send size -> SECOND: buffer)	
		write(fd, &current_length, sizeof(current_length));
		write(fd, buffer, current_length);
	}
	
	// Close file
	fclose(ptr_file);
	
	return length;
}
