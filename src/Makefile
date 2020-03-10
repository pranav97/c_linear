all: cmatrix

cmatrix: clean matrix
	
CC=gcc
CCFLAGS=-Wall -g

matrix: main.o matrix.o
	$(CC) $(CCFLAGS) -pthread main.o matrix.o -o matrix 

clean:
	rm -f matrix matrix.o main.o
