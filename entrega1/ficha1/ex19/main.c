/*
 * Renato Oliveira > 1140822@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * main.c
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpy_file.h"

#define MAX_SIZE 255

/*
 * Module 1 - Exercise 19
 */

int main(void) {
	int loop=1;
	int ret=1;
	char * user_input =malloc(sizeof(char)* MAX_SIZE);
	while(loop==1)
	{
		printf("Insira o nome de um ficheiro para copiar para a pasta backup.\n0 para terminar.\n");
		scanf("%s",user_input);
		if(strcmp(user_input,"0")==0)
		{
			break;
		}
		ret = copy_file(user_input);
	}
	free(user_input);
	return ret;
}
