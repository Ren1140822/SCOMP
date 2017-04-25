/*
 * Renato Oliveira > 1140822@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * withdraw.c
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
#include <semaphore.h> 
#include "shd_type.h"



#define NUM_SEMS 3

const char *SEM_NAME[NUM_SEMS] = {"sem_request", "sem_process","sem_mutex"};
const int NR_SEM_REQUEST =0;
const int NR_SEM_PROCESS =1;
const int MUTEX =2;

/*
 * PL 4 - Exercise 08
 */
int main(int argc, char *argv[])
{
	int client_number,balance,ammount;
	int fd, data_size = sizeof(shd_type);
	shd_type *sh_data;
	sem_t *sems[NUM_SEMS];
	// Create mutual exclusion semaphore
	sems[NR_SEM_REQUEST] = sem_open(SEM_NAME[NR_SEM_REQUEST], 0); 
	sems[NR_SEM_PROCESS] = sem_open(SEM_NAME[NR_SEM_PROCESS], 0); 
	sems[MUTEX] = sem_open(SEM_NAME[MUTEX],  O_CREAT, S_IRUSR|S_IWUSR, 1); 
	if (sems[NR_SEM_REQUEST] == SEM_FAILED || sems[NR_SEM_PROCESS] == SEM_FAILED || sems[MUTEX]  == SEM_FAILED)
	{
		perror("Semaphore failed.\n");
		exit(EXIT_FAILURE);
	}
	
	fd = shm_open(SHM_NAME, O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
	if (fd < 0)
	{
		perror("Error opening shared memory.");
		exit(EXIT_FAILURE);
	}
	// Truncate shm
	if (ftruncate(fd, data_size) < 0)
	{
		perror("Truncating shared memory failed.");
		exit(EXIT_FAILURE);
	}
	// Map shm
	sh_data = (shd_type *) mmap(NULL, data_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if (sh_data == MAP_FAILED)
	{
		perror("Mapping shared memory failed.");
		exit(EXIT_FAILURE);
	}
	
	printf("Insert client number.\n");
	scanf("%d",&client_number);
	while(1)
	{
		if(client_number!=0)
		{
			printf("Insert the ammount to withdraw, 0 to retrieve card.\n");
			scanf("%d",&ammount);
		}
		
		//MUTEX para proteger varios clientes de alterarem os dados da area partilhada ao mesmo tempo
		sem_wait(sems[MUTEX]);
		
		//altera os dados da area partilhada
		sh_data->op_type=1;
		sh_data->client_number= client_number;
		sh_data->withdraw_ammount= ammount; 
		
		//quando está preparado, acorda o servidor, que está à espera
		sem_post(sems[NR_SEM_REQUEST]);
		
		//fica à espera que o servidor processe os dados e coloque a resposta no sitio certo
		sem_wait(sems[NR_SEM_PROCESS]);
		
		if(ammount!=0 && sh_data->close_connection!=1)
		{
			balance = sh_data->balance;
			printf("You have %d balance.\n",balance);
			//liberta o semaforo de excusao mutua
			sem_post(sems[MUTEX]);
		}
		else
		{
		
			printf("%s",sh_data->close_connection!=1?"Retrieving card...\n":"Client not found on server.\n");
			sh_data->close_connection=0;//resets the connection state anyway.
			sem_post(sems[MUTEX]);
		
			break;
		}
		
		
	}
	// Close Semaphore
	int i;
	for (i = 0; i < NUM_SEMS; i++) 
	{
		// Close Semaphore
		if (sem_close(sems[i]) < 0)
		{
			perror("SEM close failed.\n");
			exit(EXIT_FAILURE);
		}
		
	}
	// Unlink Semaphore
	if (sem_unlink(SEM_NAME[MUTEX]) < 0)
	{
		perror("SEM unlink failed.\n");
		exit(EXIT_FAILURE);
	}
	
	return 0;
}
