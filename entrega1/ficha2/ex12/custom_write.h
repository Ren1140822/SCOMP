/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * custom_write.h
 * 
 */

/*
 * Write to pipe content of a file.
 * 
 * returns the file length.
 */
int write_to_pipe(int fd, const char *filename, const unsigned int buffer_size);
