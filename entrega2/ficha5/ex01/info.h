/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
 * Renato Oliveira > 1140822@isep.ipp.pt
 * 
 * SCOMP - Turma 2DD
 * 
 * info.h
 */
 
 
#ifndef INFO_H
#define INFO_H

#define BUFFER_SIZE 81

/*
 * struct with the required info
 */
typedef struct
{
	int number;
	char name[BUFFER_SIZE];
	char address[BUFFER_SIZE];
}
info;
#endif
