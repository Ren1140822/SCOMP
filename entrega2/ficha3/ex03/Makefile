#
# Renato Oliveira > 1140822@isep.ipp.pt
# SCOMP - Turma 2DD
#
# Makefile
#

# Variables
INCLUDES = 
SOURCES = program.c
OBJFILES = program.o
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
