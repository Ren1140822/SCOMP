#
# Daniel Goncalves > 1151452@isep.ipp.pt
# SCOMP - Turma 2DD
#
# Makefile
#

all: clean exhibitor visitor

exhibitor:
	gcc -Wall -g -o exhibitor exhibitor.c sem_array.c -lrt -lm -lpthread

visitor:
	gcc -Wall -g -o visitor visitor.c sem_array.c -lrt -lm -lpthread

clean:
	rm -f exhibitor visitor
