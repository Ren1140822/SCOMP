/*
 * Renato Oliveira > 1140822@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * operation.c
 * 
 */
#include <stdio.h>
#include <stdlib.h>

/*
 * 
 * Checks if two strings can be read the same backwards.
 */
int palindromes(char *vec,unsigned int size)
{
	 printf("%c=",*(vec));
	 printf("%c\n",*(vec+size));
	    
	if(vec>=vec+size)
	{
		return 1;
	}
	else if(*(vec)==*(vec+size)){
		
		 return palindromes(vec+1,size-2);
	}
	else if(*(vec)==' ') {
		return palindromes(vec+1,size-1);
		
	}
	else if(*(vec+size)==' ')
	{
		return palindromes(vec,size-1);
	}
	return 0;
		
}
