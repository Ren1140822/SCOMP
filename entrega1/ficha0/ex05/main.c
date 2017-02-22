/*
 * Renato Oliveira > 1140822@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * main.c
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include "invert_vec.h"

/*
 * Module 0 - Exercise 2
 */

#define SIZE 5

int main(void) {
	
	 int vec[SIZE] = {5,4,3,2,1};
	 float vec_inv[SIZE];
	 
	 int result = invert_vec(vec,SIZE,vec_inv);
	  
	  if(result>0)
	  {
		print_vec(vec_inv,SIZE);
	  }
	  else{
		  printf("Não foi possivel inverter, um valor era nulo.\n");
	  }
	 return 0;
}
