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

/*
 * Module 1 - Exercise 9
 */

int main(void) {
	
	int i;
	int  vec[1200000];
	for(i = 0;i<1200000;i++)
	{
		*(vec+i)=i+1;
	}
	pid_t pid_a,pid_b,pid_c,pid_d,pid_e;
	write_vector(vec,0);
	pid_a= fork();
	
	if(pid_a>0)
	{	
		pid_b=fork();
		if(pid_b>0)
		{
			pid_c=fork();
			if(pid_c>0)
			{
				pid_d=fork();
				
				if(pid_d>0)
				{
					pid_e=fork();
				}
			}
		}
	}
	if(pid_a==0)
	{
		write_vector(vec,200000);
		exit(0);
	}
	if(pid_b==0)
	{
		
		write_vector(vec,400000);
		
		exit(0);
	}
	if(pid_c==0)
	{
		
		write_vector(vec,600000);
		
		exit(0);
	}
	if(pid_d==0)
	{
		
		write_vector(vec,800000);
		
		exit(0);
	}
	if(pid_e==0)
	{
		
		write_vector(vec,1000000);
		exit(0);
		
	}
	
	if(pid_a>0)//redundante, qualquer outro processo já tinha saido.
	{
		 wait(NULL);
		 exit(0);
		 
	}
	return 0;
}
