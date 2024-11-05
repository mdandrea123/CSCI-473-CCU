CC = gcc-14
CFLAGS = -fopenmp -Wall
TARGETS = make-2d omp-matrix-vector print-vector write-vector
UTILS = functions.o

all: $(TARGETS)

make-2d: make-2d.o $(UTILS)
	$(CC) $(CFLAGS) -o $@ make-2d.o $(UTILS)

omp-matrix-vector: omp_matrix_vector.o $(UTILS)
	$(CC) $(CFLAGS) -o $@ omp_matrix_vector.o $(UTILS)

print-vector: print-vector.o $(UTILS)
	$(CC) $(CFLAGS) -o $@ print-vector.o $(UTILS)

write-vector: write-vector.o $(UTILS)
	$(CC) $(CFLAGS) -o $@ write-vector.o $(UTILS)

functions.o: functions.c
	$(CC) $(CFLAGS) -c functions.c

make-2d.o: make-2d.c
	$(CC) $(CFLAGS) -c make-2d.c

omp_matrix_vector.o: omp_matrix_vector.c
	$(CC) $(CFLAGS) -c omp_matrix_vector.c

print-vector.o: print-vector.c
	$(CC) $(CFLAGS) -c print-vector.c

write-vector.o: write-vector.c
	$(CC) $(CFLAGS) -c write-vector.c

clean:
	rm -f $(TARGETS) *.o *.dat

.PHONY: all clean
