/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * shd_type.h
 */
 
 
#ifndef SHD_TYPE_H
#define SHD_TYPE_H

#define SHD_VEC_SIZE 10
#define SHM_NAME "/shm_f4_ex08"

/*
 * struct for a shared data type
 */
typedef struct
{
	int client_number;
	int balance;
}
shd_type;

#endif
