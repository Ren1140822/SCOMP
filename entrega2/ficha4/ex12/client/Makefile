#
# Daniel Goncalves > 1151452@isep.ipp.pt
# SCOMP - Turma 2DD
#
# Makefile
#

# Variables
INCLUDES = 
SOURCES = client.c 
OBJFILES = client.o 
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
