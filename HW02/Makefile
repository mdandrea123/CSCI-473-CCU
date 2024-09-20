CFLAGS=-g -Wall -Wstrict-prototypes
PROGS=mpi_trap
OBJECTS=mpi_trap.o trap.o
LDFLAGS=-lm
CC=gcc
MCC=mpicc

all: $(PROGS)

mpi_trap: $(OBJECTS)
	$(MCC) $(LDFLAGS) -o mpi_trap $(OBJECTS)

mpi_trap.o: mpi_trap.c
	$(MCC) $(CFLAGS) -c mpi_trap.c

trap.o: trap.c trap.h
	$(MCC) $(CFLAGS) -c trap.c

clean:
	rm -f $(PROGS) *.o core* *.txt *.png
