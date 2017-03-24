/*
 * Renato Oliveira > 1140822@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * concat_string_with_int.c
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* concat_string_with_int(char* word1,int number )
{
	char int_converted [81];	
	sprintf(int_converted,"%d",number);
	
	char * concat_word = (char*)malloc(sizeof(word1)+sizeof(int_converted));
	int word_size = sizeof(word1);
	strncat(concat_word,word1,word_size);
	word_size = sizeof(int_converted);
	strncat(concat_word,int_converted,word_size);
	
	return concat_word;
}
