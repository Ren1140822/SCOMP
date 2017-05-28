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
#include <time.h>
#include <string.h>

// Setting constants
#define MAX_CLIENTS 100
#define NUM_CLIENTS 20
#define IP_SIZE 16
#define BUFF_SIZE 512
// Enum Helpers
enum mutex = {signup, client_index, chat, censure, broadcast, mutex_size};
// Client Info struct
typedef struct
{
	unsigned int id;
	string ip[IP_SIZE]; // TODO
} client;
// Message struct
typedef struct
{
	unsigned int id;
	char msg[BUFF_SIZE];
} message;
// Global vars
client registered_clients[MAX_CLIENTS];
client signup_request;
unsigned int next_client = 0;
unsigned int received_messages = 0;
message messages_sent[NUM_CLIENTS];
message messages_received[NUM_CLIENTS][NUM_CLIENTS]; // ignore itself
pthread_mutex_t mutexs[mutex_size];
pthread_cond_t cond[mutex_size];
// Error constants
const int THREAD_CREATE_ERROR = -1;
const int THREAD_JOIN_ERROR = -2;
const int THREAD_EXIT_ERROR = -3;
const int MUTEX_INIT_ERROR = -4;
const int MUTEX_DESTROY_ERROR = -5;
// Thread function
void *client_thread(void *param);
void *signup_server(void);
void *chat_server(void *param);
void *censure_entity(void *param)

/*
 * PL 5 - Exercise 10
 */
int main(void)
{
	// Vars
	client registered_clients[MAX_CLIENTS]; 
	
	// Init mutexs
	
	// Create Client Threads
	
	// Joins with each terminated client thread
	
	return 0;
}

// client function
void *client_thread(void *param)
{
	client info = *((client *) param); // TODO
	
	int index;
	
	// Request Signup
	pthread_mutex_lock(&mutexs[signup]);
	signup_request = info;
	pthread_cond_signal(&cond_signup); // Send signup request
	pthread_mutex_unlock(&mutexs[signup]);
	
	// Get client index
	pthread_mutex_lock(&mutexs[client_index]);
	pthread_cond_wait(&cond_client_index, &mutexs[client_index]);
	index = next_client;
	pthread_mutex_unlock(&mutexs[client_index]);
	
	// Wait for server to authorize message sending
	pthread_mutex_lock(&mutexs[chat]);
	pthread_cond_wait(&cond_chat, &mutexs[chat]);
	
	// Sends message with id
	message *my_msg = (message *) malloc(sizeof(message));
	snprintf(my_msg->msg, BUFF_SIZE, "message from %d", index);
	my_msg->id = index;
	messages_sent[index - 1] = my_msg;
	
	// Wait for all clients to write and reads his feed
	pthread_mutex_lock(&mutexs[read_msgs]);
	pthread_cond_wait(&cond_read_msgs, &mutexs[read_msgs]);
	
	printf("*******			CLIENT #%d		********", index);
	for (i = 0; i < NUM_CLIENTS; i++)
	{
		if (i != index-1)
		{
			printf("%s [from: %d]", messages_received[index-1][i]->msg, messages_received[index-1][i]->id);
		}
	}
	printf("*****************************************", index);
	
	pthread_exit(NULL); // terminates
}
// register function
void *signup_server(void)
{
	int clients_registered = 0;
	while (clients_registered < NUM_CLIENTS) // TODO
	{
		// Request Signup
		pthread_mutex_lock(&mutexs[signup]);
		pthread_cond_wait(&cond_signup, &mutexs[signup]);
		
		pthread_mutex_lock(&mutexs[client_index]);
		next_client++;
		pthread_cond_signal(&cond_client_index); // signal client to get client index
		pthread_mutex_unlock(&mutexs[client_index]); // Unlock client_index
		
		pthread_mutex_unlock(&mutexs[signup]); // Unlock signup for next client
		
		clients_registered++;
	}
}
// server function
void *chat_server(void *param)
{
	// Opens communications
	pthread_mutex_lock(&mutexs[chat]);
	pthread_cond_broadcast(&cond_chat); // Signals all clients
	pthread_mutex_unlock(&mutexs[chat]);
	
	// Broadcast messages
	pthread_mutex_lock(&mutexs[broadcast]);
	pthread_cond_wait(&cond_broadcast, &mutexs[broadcast]);
	pthread_mutex_unlock(&mutexs[broadcast]);
	
	int i;
	for (i = 0; i < NUM_CLIENTS; i++)
	{
		// TOOD: Send broadcast messages
	}
}

// censure entity function
void *censure_entity(void *param)
{
	// Waits for all messages
	pthread_mutex_lock(&mutexs[censure]);
	while (sent_messages < NUM_CLIENTS)
	{
		pthread_cond_wait(&cond_censure, &mutexs[censure]);
	}
	int i;
	for (i = 0; i < NUM_CLIENTS; i++)
	{
		// Verify if message should be censured 
	}
	pthread_mutex_unlock(&mutexs[censure]);
	
	// Signal server to start sending in broadcast
	pthread_mutex_lock(&mutexs[broadcast]);
	pthread_cond_signal(&cond_broadcast); 
	pthread_mutex_unlock(&mutexs[broadcast]);
	
	pthread_exit(NULL);
}
	












