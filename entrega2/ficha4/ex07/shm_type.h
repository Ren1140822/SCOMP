/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * shm_type.h
 */
 
 
#ifndef SHM_TYPE_H
#define SHM_TYPE_H

#define SHM_NAME "/shm_f4_ex07"

/*
 * struct for a shared data type
 */
typedef struct
{
	unsigned int ticket; // Next ticket to sell
	unsigned int waiting_ticket; // Next waiting ticket
	unsigned int next; // Next client to be atended
	unsigned int queue_size; // Size of the waiting line
}
shm_type;

#endif
