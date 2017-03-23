#
#
#Renato Oliveira > 1140822@isep.ipp.pt
#SCOMP - Turma 2DD
# Makefile
#

# variables
INCLUDES = create_childs.h concat_string_with_int.h
SOURCES = main.c create_childs.c concat_string_with_int.c
OBJFILES = main.o create_childs.o concat_string_with_int.o
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
