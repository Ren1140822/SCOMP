/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
 * Renato Oliveira > 1140822@isep.ipp.pt
 * 
 * SCOMP - Turma 2DD
 * 
 * thread_func.c
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>

#include "info.h"

void *print_info(void *information)
{
	
	info *printable = (info *) information;
	
	printf("INFO: %d | %s | %s\n", printable->number, printable->name, printable->address);
	
	pthread_exit(NULL);
}
