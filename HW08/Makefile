# Compiler and flags
MPICC = mpicc
CC = gcc
CFLAGS = -Wall -g

# Source files
SRC = make-2d.c MyMPI.c utilities.c test-mpi-read.c print-2d.c

# Object files (automatically generated from source files)
OBJ = $(SRC:.c=.o)

# Header files
HEADERS = MyMPI.h utilities.h

# Executables
MAKE2D = make-2d
TESTMPI = test-mpi-read
PRINT2D = print-2d

# Default target
all: $(MAKE2D) $(TESTMPI) $(PRINT2D)

# Rule for make-2d executable
make-2d: make-2d.o utilities.o MyMPI.o
	$(MPICC) $(CFLAGS) -o $@ $^

# Rule for test-mpi-read executable
test-mpi-read: test-mpi-read.o MyMPI.o utilities.o
	$(MPICC) $(CFLAGS) -o $@ $^

# Rule for print-2d executable (needs MyMPI.o for print_submatrix)
print-2d: print-2d.o utilities.o MyMPI.o
	$(MPICC) $(CFLAGS) -o $@ $^

# Generic rule for object files
%.o: %.c $(HEADERS)
	$(MPICC) $(CFLAGS) -c $<

# Clean target
clean:
	rm -f $(OBJ) $(MAKE2D) $(TESTMPI) $(PRINT2D) *.in *.out *.dat *.log

.PHONY: all clean