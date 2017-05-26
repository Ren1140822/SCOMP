/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
 * Renato Oliveira > 1140822@isep.ipp.pt
 * 
 * SCOMP - Turma 2DD
 * 
 * info.h
 */
 
 
#ifndef INFO_H
#define INFO_H
#define SIZE 3


/*
 * struct with the required info
 */
typedef struct
{
	int id_h;
	int id_p;
	int preco;
}
info;

typedef struct
{
	info * information;
	int thread_nr;
}
data;
typedef struct
{
	int id_h;
	int preco;
}
winner_market;
#endif
