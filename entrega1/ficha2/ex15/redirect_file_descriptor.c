/*
 * Renato Oliveira > 1140822@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * redirect_file_descriptor.c
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

void redirect_file_descriptor(int my_descriptor,int descriptor_to_redirect_to_mine)
{
	if(dup2(my_descriptor,descriptor_to_redirect_to_mine)<0)
	{
		exit(-1);
	}
	
}
