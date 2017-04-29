/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * file_helper.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

const int BUFFER_SIZE = 81;
const char *TEMP_FILE = "temp.tmp";

int remove_line(char *filename)
{
	
	FILE *f_in = fopen(filename, "r");
	FILE *f_out = fopen(TEMP_FILE, "a");
	if (f_in == NULL || f_out == NULL)
	{
		return -1;
	}
	
	int line_no = 0;
	char buffer[BUFFER_SIZE];
	while ( fgets(buffer, BUFFER_SIZE, f_in) != NULL )
	{
		if (line_no > 0)
		{
			fputs(buffer, f_out);
		}
		
		line_no += ( buffer[strlen(buffer) - 1] == '\n' ) ? 1 : 0; // Check if first line
		
		memset(buffer,0,strlen(buffer)); // clear buffer
	}

	if ( (fclose(f_in) == EOF) | (fclose(f_out) == EOF) ) 
	{
		return -1;
	}
    //rename the temp file to filename
    rename(TEMP_FILE, filename);
    
    return 1;
}

int count_lines(char *filename)
{
	FILE *fptr = fopen(filename, "r");
	if (fptr == NULL)
	{
		return -1;
	}
	
	int counter = 0;
	char buffer[BUFFER_SIZE];
	while ( fgets(buffer, BUFFER_SIZE, fptr) != NULL )
	{
		counter += ( buffer[strlen(buffer) - 1] == '\n' ) ? 1 : 0; // Count lines
	}

	if (fclose(fptr) == EOF) 
	{
		return -1;
	}
    
    return counter;
}

