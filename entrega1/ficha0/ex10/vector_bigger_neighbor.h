/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * vector_bigger_neighbor.h
 * 
 */

/*
 * Crie dinamicamente um vector para armazenar n inteiros (o valor de n 
 * escolhido pelo utilizador). Implemente uma função que recebendo o 
 * endereço desse vector e o seu tamanho, coloque num segundo vector 
 * todos os elementos que são maiores que os seus vizinhos. O tamanho 
 * final deste segundo vector deve ser igual ao seu número de elementos.
 */
int* vector_bigger_neighbor(int *vec, unsigned int size, unsigned int *peeks_size);
