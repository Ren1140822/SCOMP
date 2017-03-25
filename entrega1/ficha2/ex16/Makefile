#
# Daniel Goncalves > 1151452@isep.ipp.pt
# SCOMP - Turma 2DD
#
# Makefile
#

# variables
INCLUDES = create_childs.h
SOURCES = injecta.c create_childs.c
OBJFILES = injecta.o create_childs.o
EXEC = injecta

${EXEC}: ${OBJFILES}
	gcc -Wall -g -o ${EXEC} ${OBJFILES}

# How to transform .c to .o ; $< -- file name
.c.o:
	gcc -Wall -g -c $<

${OBJFILES}: ${SOURCES} ${INCLUDES}

run: ${EXEC}
	./${EXEC}

debug: ${EXEC}
	gdb -tui ${EXEC}

clean:
	rm -f ${OBJFILES} ${EXEC}
	
# external programs too
all: programa1 programa2 ${EXEC}

programa1:
	gcc -Wall -g -o programa1 programa1.c

programa2:
	gcc -Wall -g -o programa2 programa2.c
	
call:
	rm -f ${OBJFILES} ${EXEC} programa1 programa2
