CC = mpicc
CFLAGS = -g -Wall -Wstrict-prototypes
LDFLAGS = -lm
PROGS = make-2d test-mpi-read
OBJECTS_MAKE_2D = make-2d.o utilities.o MyMPI.o
OBJECTS_TEST_MPI_READ = test-mpi-read.o utilities.o MyMPI.o

all: $(PROGS)

make-2d: $(OBJECTS_MAKE_2D)
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS_MAKE_2D)

test-mpi-read: $(OBJECTS_TEST_MPI_READ)
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS_TEST_MPI_READ)

make-2d.o: make-2d.c
	$(CC) $(CFLAGS) -c make-2d.c

test-mpi-read.o: test-mpi-read.c
	$(CC) $(CFLAGS) -c test-mpi-read.c

utilities.o: utilities.c utilities.h
	$(CC) $(CFLAGS) -c utilities.c

MyMPI.o: MyMPI.c MyMPI.h
	$(CC) $(CFLAGS) -c MyMPI.c

clean:
	rm -f $(PROGS) *.o core* *.out *.in *.txt
