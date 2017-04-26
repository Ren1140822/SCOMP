#
# Daniel Goncalves > 1151452@isep.ipp.pt
# SCOMP - Turma 2DD
#
# Makefile
#

all: clean seller client

seller:
	gcc -Wall -g -o seller seller.c sem_array.c -lrt -lm -lpthread

client:
	gcc -Wall -g -o client client.c sem_array.c -lrt -lm -lpthread

clean:
	rm -f client seller
