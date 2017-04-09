/*
 * Renato Oliveira > 1140822@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * shd_type.h
 */
 
 
#ifndef SHD_TYPE_H
#define SHD_TYPE_H

#define SHD_VEC_SIZE 10
#define SHM_NAME "/shm_f3_ex07"

/*
 * struct for a shared data type
 */
typedef struct
{
	int  writer_index;
	int reader_index;
	int numbers_to_read;
	int vector[SHD_VEC_SIZE];
	
}
shd_type;

#endif
