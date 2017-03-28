/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * shd_type.h
 */
 
 
#ifndef SHD_TYPE_H
#define SHD_TYPE_H

#define SHD_VEC_SIZE 10
#define SHM_NAME "/shm_f3_ex02"

/*
 * struct for a shared data type
 */
typedef struct
{
	int w_flag; // flag to check when finished writing (Set to 1 when written).
	int vector[SHD_VEC_SIZE];
}
shd_type;

#endif
