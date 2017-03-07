/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * main.c
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int create_childs(int num)
{
	pid_t pid;
	int i;
	for (i = 1; i <= num; i++)
	{
		pid = fork();
		if (pid < 0)
		{
			return -1;
		}
		if (pid == 0)
		{
			return i;
		}
	}
	return 0;
}
