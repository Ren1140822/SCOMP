#
# Daniel Goncalves > 1151452@isep.ipp.pt
# SCOMP - Turma 2DD
#
# Makefile
#

# Variables
INCLUDES = create_childs.h circular_buffer.h sem_array.h
SOURCES = main.c create_childs.c circular_buffer.c sem_array.c
OBJFILES = main.o create_childs.o circular_buffer.o sem_array.o
EXEC = exec

${EXEC}: ${OBJFILES}
	gcc -Wall -g -o ${EXEC} ${OBJFILES} -lrt -lm -lpthread

# How to transform .c to .o ; $< -- file name
.c.o:
	gcc -Wall -g -c $< -lrt -lm -lpthread

${OBJFILES}: ${SOURCES} ${INCLUDES}

run: clean ${EXEC}
	./${EXEC}
	
debug: ${EXEC}
	gdb -tui ${EXEC}

clean:
	rm -f ${OBJFILES} ${EXEC}
