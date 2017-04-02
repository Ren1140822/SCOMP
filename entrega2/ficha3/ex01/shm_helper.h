/*
 * Renato Oliveira > 1140822@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * shm_helper.h
 * 
 */

/*
 * Open shared memory in reader's mode only. The functions waits (ACTIVE)
 * until writer creates shared memory file.
 * 
 * NOTE: Active wait with a timeout (in seconds) limit.
 */ 
int shm_open_rd_mode(const char *name, unsigned int timeout);

/*
 * Waits until flag is set to 1.
 */
int wait_for_writer(int *w_flag, int timeout);
