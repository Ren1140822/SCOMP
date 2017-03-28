/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * shm_helper.c
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */

// Settings constants
const unsigned int SLEEP_TIME = 1;

/*
 * Open shared memory in reader's mode only. The functions waits (ACTIVE)
 * until writer creates shared memory file.
 * 
 * NOTE: Active wait with a timeout (in seconds) limit.
 */ 
int shm_open_rd_mode(const char *name, unsigned int timeout)
{
	int fd, counter = 0;
	
	fd = shm_open(name, O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
	while( (fd < 0) && (counter < timeout) )
	{
		sleep(SLEEP_TIME);
		counter += SLEEP_TIME;
		fd = shm_open(name, O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
	}
	return fd;
}

/*
 * Waits until flag is set to 1.
 */
int wait_to_write(int *w_flag, int timeout)
{
	int counter = 0;
	while( (*w_flag != 1) && (counter < timeout) )
	{
		sleep(SLEEP_TIME);
		counter += SLEEP_TIME;
	}
	return (counter < timeout) ? 1 : -1;
}  
