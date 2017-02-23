/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * somabyte2.h
 * 
 */
#include <stdio.h>
#include <stdlib.h>

#define clearbyte2_mask(x) 				(0xFFFF00FF & x)
#define pastebyte2_mask(orig, dest)		((orig << 8) ^ clearbyte2_mask(dest))

/*
 * Implemente a função void somabyte2 (char x, int *vec1, int *vec2) que recebe 
 * como parâmetros o endereço de dois vectores e um char x. A função deve 
 * adicionar x ao segundo byte menos significativo de cada um dos elementos 
 * de vec1 e armazenar o resultado desse operação em vec2. Assuma que o 
 * primeiro elemento de vec1 indica o tamanho de ambos os vectores.
 */
void somabyte2(char x, int *vec1, int *vec2)
{
	int i;
	for (i = 1; i < *vec1; i++)
	{
		vec2[i - 1] = pastebyte2_mask(x, *(vec1 + i));
	}
}
