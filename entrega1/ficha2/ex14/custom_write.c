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

const int BUFF_SIZE = 81;

/*
 * Write to pipe a sequence of lines.
 */
void write_to_pipe(int fd, int n_lines)
{
	// Initialize buffer
	char buffer[BUFF_SIZE];
	
	// Send lines
	int line;
	for (line = 1; line <= n_lines; line++)
	{
		sprintf(buffer, "Line %d\n", line);
		write(fd, buffer, strlen(buffer)); 
	}
}
