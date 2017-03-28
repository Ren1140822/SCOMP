#
# Daniel Goncalves > 1151452@isep.ipp.pt
# SCOMP - Turma 2DD
#
# Makefile
#

# Variables WRITER
INCLUDES_W = shd_type.h shm_helper.h vector_helper.h
SOURCES_W = shm_helper.c vector_helper.c
OBJFILES_W = shm_helper.o vector_helper.o
# Variables READER
INCLUDES_R = shd_type.h shm_helper.h calc.h
SOURCES_R = shm_helper.c calc.c
OBJFILES_R = shm_helper.o calc.o

all: clean reader writer

reader: ${OBJFILES_R}
	gcc -Wall -g -o reader reader.c ${OBJFILES_R} -lrt -lm

writer: ${OBJFILES_W}
	gcc -Wall -g -o writer writer.c ${OBJFILES_W} -lrt -lm

run_reader: reader
	./reader
	
run_writer: writer
	./writer
	
clean:
	rm -f reader writer ${OBJFILES_W} ${OBJFILES_R}

# How to transform .c to .o ; $< -- file name
.c.o:
	gcc -Wall -g -c $< -lrt -lm
	
${OBJFILES_W}: ${SOURCES_W} ${INCLUDES_W}

${OBJFILES_R}: ${SOURCES_R} ${INCLUDES_R}
