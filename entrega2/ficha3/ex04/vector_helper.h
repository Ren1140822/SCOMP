/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * vector_helper.h
 */

/*
 * Function to print a vector with n integers.
 * 
 * int *vec - pointer to vector
 * unsigned int n - the number of registers to print. 
 */
void print_vec(int *vec, unsigned int size);

/*
 * Fills a vector with a n random of integers.
 */ 
void fill_with_random_ints(int *vec, unsigned int size, int min, int max);

/*
 * Returns the maximum element of the array.
 */
int find_max(int *vec, unsigned int size);
