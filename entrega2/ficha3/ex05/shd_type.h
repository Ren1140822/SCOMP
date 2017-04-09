/*
 * Renato Oliveira > 1140822@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * shd_type.h
 */
 
 
#ifndef SHD_TYPE_H
#define SHD_TYPE_H

#define STR_SIZE 30
#define NR_DISC 10
#define SHM_NAME "/shm_f3_ex05"

/*
 * struct for a shared data type
 */
typedef struct
{
	int w_flag; // flag to check when finished writing (Set to 1 when written).
	unsigned int numero;
	char nome[STR_SIZE];
	unsigned int disciplinas[NR_DISC];
	
}
shd_type;

#endif
