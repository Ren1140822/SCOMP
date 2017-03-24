/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * vector_helper.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sale.h"

/*
 * Function to print a vector with n number of sales.
 * register *sales - the sales to print.
 * unsigned int n - the number of registers to print. 
 */
void print_sales(sale *sales, unsigned int n) 
{
	printf("\n{\n");
	
	int i;
	for (i = 0; i < n; i++)
	{
		printf("cliente id: %d | product id: %d | quantity: %d%s", 
		sales[i].cliente_id, sales[i].product_id, sales[i].quantity, i != n-1 ? ";\n": "\n");
	}
	printf("}\n");
}

/*
 * Fills vector with a n sequence of registers.
 */ 
void fill_sales(sale *sales, unsigned int n)
{
	/* Intializes random number generator */
	time_t t;
	srand((unsigned) time(&t));
	
	int i;
	for (i = 0; i < n; i++)
	{
		sale temp;
		temp.cliente_id = i + 1;
		temp.product_id = i + 100;
		temp.quantity = rand() % 100;
		sales[i] = temp;
	}
}
