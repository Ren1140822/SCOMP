/*
 * Daniel Goncalves > 1151452@isep.ipp.pt
 * SCOMP - Turma 2DD
 * 
 * sem_array.h
 * 
 */
sem_t **create_sem_array(sem_t **sems, size_t SIZE, int sem_value);

int close_sem_array(sem_t **sems, size_t SIZE);

int unlink_sem_array(sem_t **sems, size_t SIZE);
