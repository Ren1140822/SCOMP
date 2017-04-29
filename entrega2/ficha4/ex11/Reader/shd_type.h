/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * shd_type.h
 */
 
 
#ifndef SHD_TYPE_H
#define SHD_TYPE_H

#define SHD_BUFFER_SIZE 81
#define SHM_NAME "/shm_f4_ex10"

/*
 * struct for a shared data type
 */
typedef struct
{
	int number_readers;
	int number_writers;
	char string[SHD_BUFFER_SIZE];
}
shd_type;
#endif
