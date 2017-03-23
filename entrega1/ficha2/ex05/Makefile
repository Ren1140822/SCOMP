#
#
#Renato Oliveira > 1140822@isep.ipp.pt
#SCOMP - Turma 2DD
# Makefile
#

# variables
INCLUDES = preenche_vetor.h create_childs.h
SOURCES = main.c preenche_vetor.c create_childs.c
OBJFILES = main.o preenche_vetor.o create_childs.o
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
