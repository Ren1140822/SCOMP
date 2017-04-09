/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * circular_buffer.h
 */
 
 
#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#define SHM_NAME "/shm_f3_ex07"
#define BUFFER_SIZE 10

/*
 * struct representing a circular buffer
 */
typedef struct
{
	int buffer[BUFFER_SIZE];
	int nxt_index;
	int count;
}
circular_buffer;

void cb_init(circular_buffer *cb);

int isFull(circular_buffer *cb);

int *write_to_cb(circular_buffer *cb, int *element);

int *read_from_cb(circular_buffer *cb, int *element);

#endif
