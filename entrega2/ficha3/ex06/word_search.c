/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * word_search.c
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const unsigned int LINE_SIZE = 201;

/*
 * Find the number of occurences of a certain word in a string.
 */
int number_of_occurences_in_string(char* str, char *word)
{
	int i = 0, counter = 0;
	while (str[i] != '\0')
	{
		if (i == 0 || str[i] == ' ')
		{
			int nxt = (i == 0) ? i : i + 1;
			if(!strncmp((str + nxt), word, strlen(word)))
			{
				counter++;
			}
		}
		i++;
	}
	return counter;
}

/*
 * Find the number of occurences of a certain word in a file.
 */
int number_of_occurences(char *file_path, char *word)
{
	FILE *file_ptr = fopen(file_path, "r");
	if(file_ptr == NULL) {
		return -1;
	}
	int counter = 0;
	char line[LINE_SIZE];
	while (fgets(line, LINE_SIZE, file_ptr) != NULL)
	{
		counter += number_of_occurences_in_string(line, word);
	}
	fclose(file_ptr);
	
	return counter;
}
