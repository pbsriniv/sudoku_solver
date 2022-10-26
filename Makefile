CC=g++
CFLAGS=-I.
DEPS = dimension.h sudoku_puzzle.h vcell.h

%.o: %.cc $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

solveit: combi.o dimension.o main.o vcell.o
	$(CC) -o solveit combi.o dimension.o vcell.o main.o

clean:
	rm -f *.o solveit
