#
# Daniel Goncalves > 1151452@isep.ipp.pt
# SCOMP - Turma 2DD
#
# Makefile
#

# Variables
INCLUDES = create_childs.h vector_helper.h
SOURCES = main.c create_childs.c vector_helper.c
OBJFILES = main.o create_childs.o vector_helper.o
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
