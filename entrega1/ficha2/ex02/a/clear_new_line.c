/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * clear_new_line.c
 * 
 */
#include <stdio.h>

/*
 * Clears the new line character ('\n').
 */
void clear_new_line(char * str)
{
	int i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '\n')
		{
			str[i] = '\0';
		}
		i++;
	}
}
