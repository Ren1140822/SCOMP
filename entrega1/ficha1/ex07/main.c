/*
 * Renato Oliveira > 1140822@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * main.c
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include "printchar.h"


/*
 * Module 0 - Exercise 7
 */

int main(void) {
	
	int i;
	pid_t pid;
	pid_t pid_f;
	for(i=0;i<2;i++)
	{
		pid=fork();
		if(pid>0)
		{
			//pai
			M("A");
		}
		else
		{
			//filho
			M("B");
			pid_f=fork();
			if(pid_f>0)
			{
				
				M("A");
			    exit(1);
			    //é necessário de acordo com o diagrama
			}
		}
	}
	return 0;
}
