/*
 * Renato Oliveira > 1140822@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * invert_vec.c
 * 
 */
#include <stdio.h>
#include <stdlib.h>

/*
 * 
 * 
 */
void invert_vec(int *vec1,unsigned int n_elements,float *vec_inv)
{
	 int i;
	 for(i=0;i<n_elements;i++)
	 {
		 if(*(vec1+i)!=0)
		 {
			*(vec_inv+i) = 1.0f/ *(vec1+i);
		 }
	 }
}
