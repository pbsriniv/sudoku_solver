
CC=g++
CFLAGS=-I.
DEPS = dimension.h vcell.h

%.o: %.cc $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

solveit: solve.o
	$(CC) -o solveit solve.o