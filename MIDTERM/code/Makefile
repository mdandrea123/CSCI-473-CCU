# Compiler and flags
CC = gcc-14
CFLAGS = -Wall -Wstrict-prototypes
LIBS = -lm

# OpenMP and Pthread flags
OMPFLAGS = -fopenmp
PTHFLAGS = -pthread

# All targets
all: make-2d print-2d stencil-2d pth-stencil-2d omp-stencil-2d 

# Object files
COMMON_OBJS = utilities.o 
PTH_OBJS = my_barrier.o

# Basic utilities
utilities.o: utilities.c utilities.h
	$(CC) $(CFLAGS) -c utilities.c

my_barrier.o: my_barrier.c my_barrier.h
	$(CC) $(CFLAGS) -c my_barrier.c

# Main programs
make-2d: make-2d.c $(COMMON_OBJS)
	$(CC) $(CFLAGS) -o make-2d make-2d.c $(COMMON_OBJS) $(LIBS)

print-2d: print-2d.c $(COMMON_OBJS)
	$(CC) $(CFLAGS) -o print-2d print-2d.c $(COMMON_OBJS) $(LIBS)

stencil-2d: stencil-2d.c $(COMMON_OBJS)
	$(CC) $(CFLAGS) -o stencil-2d stencil-2d.c $(COMMON_OBJS) $(LIBS)

# Pthread version
pth-stencil-2d: pth-stencil-2d.c $(COMMON_OBJS) $(PTH_OBJS)
	$(CC) $(CFLAGS) $(PTHFLAGS) -o pth-stencil-2d pth-stencil-2d.c $(COMMON_OBJS) $(PTH_OBJS) $(LIBS)

# OpenMP version
omp-stencil-2d: omp-stencil-2d.c $(COMMON_OBJS)
	$(CC) $(CFLAGS) $(OMPFLAGS) -o omp-stencil-2d omp-stencil-2d.c $(COMMON_OBJS) $(LIBS)

# Clean
clean:
	rm -f *.o make-2d print-2d stencil-2d pth-stencil-2d omp-stencil-2d *.dat *.raw *.csv
	rm -f ../data/*.dat ../data/*.raw ../data/*.csv ../data/*.png

.PHONY: all clean 