#
#
#Renato Oliveira > 1140822@isep.ipp.pt
#SCOMP - Turma 2DD
# Makefile
#

# variables
INCLUDES = generate_random.h
SOURCES = main.c generate_random.c
OBJFILES = main.o generate_random.o
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
