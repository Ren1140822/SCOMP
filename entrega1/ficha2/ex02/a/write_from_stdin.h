/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * write_from_stdin.h
 * 
 */


/*
 * Writes all input to pipe until newline ('\n').
 * 
 * returns total bytes written.
 */
int write_from_stdin(int fd, char *buffer, int buffer_size);
