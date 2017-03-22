/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * read_to_stdout.c
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/*
 * Reads all output from pipe to stdout.
 */
void read_to_stdout(int fd, const int buffer_size)
{
	int str_len;
	char buffer[buffer_size];
	
	while (read(fd, &str_len, sizeof(str_len)))
	{
		read(fd, buffer, str_len);
		fputs(buffer, stdout);
		// Clears buffer for next read
		memset(buffer, '\0', buffer_size);
	}
}
