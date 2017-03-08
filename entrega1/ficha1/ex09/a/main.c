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
#include <sys/wait.h>
#include "write_vector.h"
#include "create_childs.h"

/*
 * Module 1 - Exercise 9
 */
 #define VEC_SIZE 1200000
 #define VEC_STEP 200000
 #define NUMBER_OF_CHILDS 6

int main(void) {
	
	int i,child_number;
	int  vec[VEC_SIZE];
	for(i = 0;i<VEC_SIZE;i++)
	{
		*(vec+i)=i+1;
	}
	
	child_number = create_childs(NUMBER_OF_CHILDS);
		
	if(child_number<0)
	{
		perror("Error creating fork.\n");
	}
	if(child_number>0)
	{
		write_vector(vec,child_number*VEC_STEP);
		exit(0);
	}
	for(i=0;i<NUMBER_OF_CHILDS;i++)
	{
		wait(NULL);
	}	
	
	return 0;
}
