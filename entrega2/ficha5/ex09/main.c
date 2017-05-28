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
#include "generate_random.h"
#include "info.h"

// Setting constants
#define SIZE 5
#define VEC_SIZE 300
Exam exams[VEC_SIZE];
int number_of_grades=0,position_to_write=0,positives=0,negatives=0;
int written_positive=0,written_negative=0;
pthread_mutex_t mux,mux2;
pthread_cond_t thread_cond,positives_cond,negatives_cond;


void * count_positives(void *param)
{
	while(positives+negatives<VEC_SIZE-1 )//ONLY WORKS SOMETIMES 
	{
		
		pthread_mutex_lock(&mux);
		while( written_positive<1 &&(positives+negatives)<VEC_SIZE-1)
		{
			
			pthread_cond_wait(&positives_cond,&mux);
		}
		written_positive--;
		positives++;
		pthread_mutex_unlock(&mux);
		
	}
	pthread_exit(NULL);
}

void * count_negatives(void *param)
{
	while(positives+negatives<VEC_SIZE-1 )//ONLY WORKS SOMETIMES
	{
		pthread_mutex_lock(&mux);
		while(written_negative<1 )
		{
		
			pthread_cond_wait(&negatives_cond,&mux);
		}
		written_negative--;
		negatives++;
		pthread_mutex_unlock(&mux);
		
	}
	pthread_exit(NULL);
}
void fill_grades(Exam * exam)
{
	exam->student_nr =1;
	exam->grade_g1=generate_random(1,20);
	exam->grade_g2=generate_random(1,20);
	exam->grade_g3=generate_random(1,20);
}


void * func(void *param)
{
	int i;
	for (i = 0; i < VEC_SIZE ; i++)
	{
		pthread_mutex_lock(&mux);
		Exam exam;
		fill_grades(&exam);
		exams[i] = exam;
		number_of_grades++;
		
		pthread_cond_signal(&thread_cond);
		pthread_mutex_unlock(&mux);
	}

	pthread_exit(NULL);
}


//2 threads call this method
void * calculate_final_grade(void* param)
{
	while(position_to_write<VEC_SIZE-1)
	{
		pthread_mutex_lock(&mux);
		while(number_of_grades<1)
		{
			pthread_cond_wait(&thread_cond,&mux);
		}
		position_to_write++;
		exams[position_to_write-1].final_grade = (exams[position_to_write-1].grade_g1 + exams[position_to_write-1].grade_g2 + exams[position_to_write-1].grade_g3)/3;
		number_of_grades --;
		//if its positive signal the correct cond
		if(exams[position_to_write-1].final_grade>=10)
		{
			written_positive++;	
			pthread_cond_signal(&positives_cond);
		}
		else
		{
			written_negative++;
			pthread_cond_signal(&negatives_cond);
		}
		
		
		pthread_mutex_unlock(&mux);
	}
	pthread_exit(NULL);
}

/*
 * PL 5 - Exercise 3
 */
int main(int argc, char *argv[])
{
	
	time_t t;
	srand((unsigned) time(&t));
	pthread_mutex_init(&mux,NULL);
	pthread_mutex_init(&mux2,NULL);
	pthread_cond_init(&thread_cond,NULL);
	pthread_cond_init(&positives_cond,NULL);
	pthread_cond_init(&negatives_cond,NULL);
	// Create Threads
	pthread_t threads[SIZE];
    int ret,i;
    
	ret = pthread_create(&threads[0], NULL, func, (void *)NULL);
	ret = pthread_create(&threads[1], NULL, calculate_final_grade, (void *)NULL);
	ret = pthread_create(&threads[2], NULL, calculate_final_grade, (void *)NULL);
	ret = pthread_create(&threads[3], NULL, count_positives, (void *)NULL);
	ret = pthread_create(&threads[4], NULL, count_negatives, (void *)NULL);
	if (ret != 0) 
	{
		perror("Create thread failed.\n");
		exit(-1);
	}
		


	for (i = 0; i < SIZE; i++)
	{
		pthread_join(threads[i], NULL); // Joins with each terminated thread (by order array)
	}
	pthread_cond_destroy(&thread_cond);
	pthread_cond_destroy(&positives_cond);
	pthread_cond_destroy(&negatives_cond);
	printf("positives %d\n",positives);
	printf("negatives %d\n",negatives);
	return 0;
}
