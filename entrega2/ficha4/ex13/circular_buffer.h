/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * circular_buffer.h
 */
 
 
#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#define SHM_NAME "/cb_f4_ex13"
#define CB_SIZE 3
#define MSG_SIZE 81

/*
 * struct representing a circular buffer
 */
typedef struct
{
	char buffer[CB_SIZE][MSG_SIZE];
	int nxt_index;
	int count;
}
circular_buffer;

void cb_init(circular_buffer *cb);

void write_to_cb(circular_buffer *cb, char msg[MSG_SIZE]);

void read_from_cb(circular_buffer *cb, char msg[MSG_SIZE]);

#endif
