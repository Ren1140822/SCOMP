/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * shm_type.h
 */
 
 
#ifndef SHM_TYPE_H
#define SHM_TYPE_H

#define SHM_NAME "/shm_f4_ex16"

/*
 * struct for a shared data type
 */
typedef struct
{
	unsigned int order_no; // Next waiting ticket
	unsigned int next; // Next client to be atended
	unsigned int full; // boolean to check if parking lot is full
}
shm_type;

#endif
