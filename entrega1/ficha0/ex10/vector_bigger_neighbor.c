/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * vector_bigger_neighbor.c
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include "bigger_than.h"

/*
 * Crie dinamicamente um vector para armazenar n inteiros (o valor de n 
 * escolhido pelo utilizador). Implemente uma função que recebendo o 
 * endereço desse vector e o seu tamanho, coloque num segundo vector 
 * todos os elementos que são maiores que os seus vizinhos. O tamanho 
 * final deste segundo vector deve ser igual ao seu número de elementos.
 */
int* vector_bigger_neighbor(int *vec, unsigned int size, unsigned int *peeks_size)
{	
	// if only one or zero elements, there is no neighbor to compare 
	if (size <= 1)
	{
		return NULL;
	}
	
	
	// Intializes a second dynamic vector
	int *vec2_ptr = NULL;
	vec2_ptr = (int *) calloc(size, sizeof(int));
	
	if (vec2_ptr == NULL)
	{
		printf("The program encountered an error.");
		exit(EXIT_FAILURE);
	}
	
	// Iterate vec to search for values bigger than his neighbors
	int i, peeks_index = 0, temp;
	// check first element
	if (*vec > *(vec + 1))
	{
		*(vec2_ptr) = *vec;
		peeks_index++;
	}
	
	// verify elements that have left && right neighbor
	for (i = 0; i < (size - 1); i++)
	{
		temp = (*(vec + i - 1) > *(vec + i + 1)) ? *(vec + i - 1) : *(vec + i + 1);
		if (*(vec + i) > temp)
		{
			*(vec2_ptr + peeks_index) = *(vec + i);
			peeks_index++;
		}
	}
	
	// check last element
	if (*(vec + (size - 1)) > *(vec + (size - 2)))
	{
		*(vec2_ptr + peeks_index) = *(vec + (size - 1));
		peeks_index++;
	}
	
	// Update peeks vector size
	*peeks_size = peeks_index;
	
	// Reallocate second vector to correct size
	int *ptr_temp = NULL;
	ptr_temp = (int *) realloc(vec2_ptr, *peeks_size * sizeof(int));
	// check realloc return 
	if (ptr_temp != NULL)
	{
		vec2_ptr = ptr_temp;
		ptr_temp = NULL;
	}
	
	return vec2_ptr;
}
