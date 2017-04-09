/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * circular_buffer.c
 */
#include <stdio.h>
#include "circular_buffer.h"
 
void cb_init(circular_buffer *cb)
{
	cb->nxt_index = 0;
	cb->count = 0;
}

int isFull(circular_buffer *cb)
{
	return cb->count == BUFFER_SIZE;
}


int *write_to_cb(circular_buffer *cb, int *element)
{
	if (isFull(cb))
	{
		return NULL;
	}
	
	cb->buffer[cb->nxt_index] = *element;
	
	cb->nxt_index = (cb->nxt_index + 1) % BUFFER_SIZE;
	
	cb->count++;
	
	return element;
}

int *read_from_cb(circular_buffer *cb, int *element)
{
    if (!cb->count) 
    { 
		return NULL; 
	}
	
    *element = cb->buffer[ ( BUFFER_SIZE - (cb->count - cb->nxt_index) ) % BUFFER_SIZE ];
    
    cb->count--;
    
    return element;
}
