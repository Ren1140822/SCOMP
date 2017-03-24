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
 * returns 0 if a error was encountered, 1 otherwise.
 */
int write_to_pipe(int fd, const char *filename, const unsigned int buffer_size)
{
	// Intialize buffer
	char buffer[buffer_size];
	// Open txt file
	FILE *ptr_file = fopen(filename, "r");
	// if open was unsuccessful
	if (!ptr_file)
	{
		return 0;
	}
	// Iterate file lines until EOF
	while (fgets(buffer, buffer_size, ptr_file))
	{
		// write to pipe
		write(fd, buffer, strlen(buffer));
	}
	
	// Close file
	fclose(ptr_file);
	
	return 1;
}
