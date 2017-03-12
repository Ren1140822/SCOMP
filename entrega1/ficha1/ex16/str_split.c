/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * str_split.c
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

#define INITIAL_CAP 10

char** str_split(char *str, char *delimiter)
{
	// Duplicate original string
	char *temp = strdup(str);
	// Confirm initial size
	int vec_size = INITIAL_CAP;
	// Allocate memory
	char **vec_str = (char **)malloc(sizeof(char*) * vec_size);
	if (vec_str == NULL)
	{
		return NULL;
	}
	// Iterate string
	char * tok;
	tok = strtok(temp,delimiter);
	int i = 0;
	while (tok != NULL)
	{
		vec_str[i] = tok;
		i++;
		tok = strtok (NULL, delimiter);
		
		if (i >= vec_size)
		{
			char **ptr = (char **)realloc(vec_str, vec_size + INITIAL_CAP);
			if (ptr == NULL)
			{
				return NULL;
			}
			vec_str = ptr;
			vec_size += INITIAL_CAP;
		}
	}
	// Adjust size to necessary (add one index for (char *) NULL)
	char **ptr = (char **)realloc(vec_str, i + 1);
	if (ptr == NULL)
	{
		return NULL;
	}
	vec_str = ptr;
	vec_str[i] = (char *) NULL;
	
	return vec_str;
}
