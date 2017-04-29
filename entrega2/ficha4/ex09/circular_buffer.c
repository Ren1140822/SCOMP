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

void write_to_cb(circular_buffer *cb, int element)
{

	cb->buffer[cb->nxt_index] = element;
	
	cb->nxt_index = (cb->nxt_index + 1) % CB_SIZE;
	
	cb->count++;
}

int read_from_cb(circular_buffer *cb)
{
	
    int element = cb->buffer[ ( CB_SIZE - (cb->count - cb->nxt_index) ) % CB_SIZE ];
    
    cb->count--;
    
    return element;
}
