/*
 * Renato Oliveira > 1140822@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * main.c
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "create_childs.h"

#define NUMBER_OF_CHILDS 3
/*
 * Module 1 - Exercise 15
 */

int main(void) {
	
	char* commands[] = {"ps","ls","pwd"};
	int seq_number = create_childs(NUMBER_OF_CHILDS);
	if(seq_number>0)
	{
		char * args[] =   {commands[seq_number-1],(char*)NULL};
		int ret = execvp(commands[seq_number-1], args);
		exit(ret);
	}
	int i;
	for(i=0;i<NUMBER_OF_CHILDS;i++)
	{
		wait(NULL);
	}
	return 0;
	
}
