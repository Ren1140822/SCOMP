/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * operation.c
 * 
 */
#include <stdio.h>
#include <stdlib.h>

/*
 * Usando aritmética de apontadores, implemente uma função que recebendo 
 * como parâmetros o endereço de um vector de inteiros, o seu número de 
 * elementos e um determinado valor, subtraia esse valor a todos os 
 * elementos do vector.
 * 
 * RECURSIVE
 */
void operation( int* vec, unsigned int size, int x)
{
	
	if (size > 1)
	{
		operation(vec + 1, size - 1, x);
	}
	
	*vec = *vec - x;
}
