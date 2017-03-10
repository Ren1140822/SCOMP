#
# Renato Oliveira > 1140822@isep.ipp.pt
# SCOMP - Turma 2DD
#
# Makefile
#

# variables
INCLUDES = cpy_file.h
SOURCES = cpy_file.c main.c
OBJFILES = cpy_file.o main.o 
EXEC = exec

${EXEC}: ${OBJFILES}
	gcc -Wall -g -o ${EXEC} ${OBJFILES}

# How to transform .c to .o ; $< -- file name
.c.o:
	gcc -Wall -g -c $<

# How to transform .s to .o ; $< -- file name
.s.o:
	gcc -Wall -g -c $<

${OBJFILES}: ${SOURCES} ${INCLUDES}

run: ${EXEC}
	./${EXEC}
	
debug: ${EXEC}
	gdb -tui ${EXEC}

clean:
	rm -f ${OBJFILES} ${EXEC}
