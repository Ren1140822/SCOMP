#
# Renato Oliveira > 1140822@isep.ipp.pt
# SCOMP - Turma 2DD
#
# Makefile
#

# Variables WRITER
INCLUDES_W = shd_type.h shm_helper.h
SOURCES_W = shm_helper.c
OBJFILES_W = shm_helper.o
# Variables READER
INCLUDES_R = shd_type.h shm_helper.h 
SOURCES_R = shm_helper.c 
OBJFILES_R = shm_helper.o 

all: clean program


program: ${OBJFILES_W}
	gcc -Wall -g -o program program.c ${OBJFILES_W} -lrt -lm


run_program: program
	./program
	
clean:
	rm -f program ${OBJFILES_W} ${OBJFILES_R}

# How to transform .c to .o ; $< -- file name
.c.o:
	gcc -Wall -g -c $< -lrt -lm
	
${OBJFILES_W}: ${SOURCES_W} ${INCLUDES_W}

${OBJFILES_R}: ${SOURCES_R} ${INCLUDES_R}
