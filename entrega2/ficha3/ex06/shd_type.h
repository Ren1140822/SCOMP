/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * shd_type.h
 */
 
 
#ifndef SHD_TYPE_H
#define SHD_TYPE_H

#define SHM_NAME "/shm_f3_ex06"
#define BUFFER_SIZE 81

/*
 * struct for a shared data type
 */
typedef struct
{
	int rd_flag; // Flag should be one when ready to be read from.
	char path[BUFFER_SIZE];
	char word[BUFFER_SIZE];
	int occurrences;
}
shd_type;

#endif
