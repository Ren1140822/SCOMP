#
# Daniel Goncalves > 1151452@isep.ipp.pt
# SCOMP - Turma 2DD
#
# Makefile
#

# Variables
INCLUDES = create_childs.h shm_helper.h word_search.h shd_type.h
SOURCES = main.c create_childs.c shm_helper.c word_search.c
OBJFILES = main.o create_childs.o shm_helper.o word_search.o
EXEC = exec

${EXEC}: ${OBJFILES}
	gcc -Wall -g -o ${EXEC} ${OBJFILES} -lrt -lm

# How to transform .c to .o ; $< -- file name
.c.o:
	gcc -Wall -g -c $< -lrt -lm

${OBJFILES}: ${SOURCES} ${INCLUDES}

run: clean ${EXEC}
	./${EXEC}
	
debug: ${EXEC}
	gdb -tui ${EXEC}

clean:
	rm -f ${OBJFILES} ${EXEC}
