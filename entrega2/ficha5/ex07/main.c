/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
 * Renato Oliveira > 1140822@isep.ipp.pt
 * 
 * SCOMP - Turma 2DD
 * 
 * main.c
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <pthread.h>
#include <string.h>
#include <limits.h>
#include "generate_random.h"
#include "info.h"

// Setting constants
#define T_NUMBER 6
#define NUMBER_PRODUCTS 15
#define H_NUMBER 3

//global vars
data market_data[H_NUMBER];
info information[NUMBER_PRODUCTS];
info vec1[NUMBER_PRODUCTS];
info vec2[NUMBER_PRODUCTS];
info vec3[NUMBER_PRODUCTS];
winner_market lowest_price_market;
pthread_mutex_t mux;

void * calculate_lowest(void *param)
{
	
	int thread_nr = (int *) param;
	
	int i;
	int local_sum=0;
	printf("%d\n",thread_nr);
	switch(thread_nr)
	{
		case 1:
		for (i = 0; i < NUMBER_PRODUCTS ; i++)
		{
			if(market_data[thread_nr-1].information[i].id_h == thread_nr)
			{
				local_sum += vec1[i].preco;
			}
		}
		printf("%d\n",local_sum);
		break;
		case 2:
		for (i = 0; i < NUMBER_PRODUCTS ; i++)
		{
			if(market_data[thread_nr-1].information[i].id_h == thread_nr)
			{
				local_sum += vec2[i].preco;
			}
		}
		printf("%d\n",local_sum);
		break;
		case 3:
		for (i = 0; i < NUMBER_PRODUCTS ; i++)
		{
			if(market_data[thread_nr-1].information[i].id_h == thread_nr)
			{
				local_sum += vec3[i].preco;
			}
		}
		printf("%d\n",local_sum);
		break;
	}
	//checks if local sum of prices is less than the lowest
	pthread_mutex_lock(&mux);
	if(local_sum<lowest_price_market.preco)
	{
		lowest_price_market.preco=local_sum;
		lowest_price_market.id_h = thread_nr;
	}
	pthread_mutex_unlock(&mux);
	pthread_exit(NULL);
}
void *func(void *param)
{
	data *current_data;
	current_data = (data*)param;
	int thread_nr = current_data->thread_nr;
	int i;
	//thread 1 fills vec1 information for supermarket 1 and so on
	switch(thread_nr)
	{
		case 1:
		for (i = 0; i < NUMBER_PRODUCTS ; i++)
		{
			if(current_data->information[i].id_h == thread_nr)
			{
				vec1[i]=current_data->information[i];
			
				 
			}
		}
		break;
		case 2:
		for (i = 0; i < NUMBER_PRODUCTS ; i++)
		{
			if(current_data->information[i].id_h == thread_nr)
			{
				vec2[i]=current_data->information[i];
				 
			}
		}
		break;
		case 3:
		for (i = 0; i < NUMBER_PRODUCTS ; i++)
		{
			if(current_data->information[i].id_h == thread_nr)
			{
				vec3[i]=current_data->information[i];
				 
			}
		}
		break;
	}
	
	pthread_t thread;
	int ret;
	//Each thread creates another new thread, passing its number as param.
	//Creating all 6 threads at start is unneeded, because these last three can only run after the first three.
	ret = pthread_create(&thread, NULL, calculate_lowest, (void *)thread_nr);

	if (ret != 0) 
	{
		perror("Create thread failed.\n");
		exit(-1);
	}
		
	
	
	
	pthread_join(thread, NULL); 
	
	pthread_exit(NULL);
}



/*
 * PL 5 - Exercise 7
 */
int main(int argc, char *argv[])
{
	
	time_t t;
	srand((unsigned) time(&t));
    lowest_price_market.preco = INT_MAX;
    pthread_mutex_init(&mux,NULL);
	int i;
	//Fill vector
	
	for (i = 0; i < NUMBER_PRODUCTS; i++)
	{
		int random = generate_random(1,3);
		
		information[i].id_h =random;
	
		information[i].id_p =i+1;
		information[i].preco=10;
	
	}
	
	
	// Create Threads
	pthread_t threads[H_NUMBER];
    int ret;
    for (i = 0; i < H_NUMBER; i++)
    {
		//prepares thread number to send to new thread
		market_data[i].thread_nr=i+1;
		//each thread will have acess to all 15 products
		market_data[i].information = information;
		ret = pthread_create(&threads[i], NULL, func, (void *)&market_data[i]);
		if (ret != 0) 
		{
			perror("Create thread failed.\n");
			exit(-1);
		}
		
	}
	
	for (i = 0; i < SIZE; i++)
	{
		pthread_join(threads[i], NULL); // Joins with each terminated thread (by order array)
	}
	printf("Lowest price is %d, found in market nr %d.\n",lowest_price_market.preco,lowest_price_market.id_h);
	return 0;
}
