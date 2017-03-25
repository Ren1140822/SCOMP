/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * programa1.c
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
 * PL 2 - Exercise 16 - Programa 2
 */
int main(void)
{
	int temp = 0, sum = 0;
	do
	{
		scanf("%d*c", &temp);
		if (temp > 0)
		{
			printf("%d\n", temp);
			sum += temp;
		}
	} while (temp > 0);
	
	sleep(1);
	printf("\n+======+\n\nSUM : %d\n", sum);
	
	return 0;
}
