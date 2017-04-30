#
# Daniel Goncalves > 1151452@isep.ipp.pt
# SCOMP - Turma 2DD
#
# Makefile
#

all: clean parking_lot car

parking_lot:
	gcc -Wall -g -o parking_lot parking_lot.c sem_array.c -lrt -lm -lpthread

car:
	gcc -Wall -g -o car car.c sem_array.c -lrt -lm -lpthread

clean:
	rm -f parking_lot car
