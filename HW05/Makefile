MPICC = mpicc
CC = gcc
CFLAGS = -g -Wall -Wstrict-prototypes -O3
LDFLAGS = -lm -O3
PROGS = nt-serial nt-parallel
OBJECTS_NT_SERIAL = nt-serial.o 
OBJECTS_NT_PARALLEL = nt-parallel.o 

all: $(PROGS)

nt-serial: $(OBJECTS_NT_SERIAL)
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS_NT_SERIAL)

nt-parallel: $(OBJECTS_NT_PARALLEL)
	$(MPICC) $(LDFLAGS) -o $@ $(OBJECTS_NT_PARALLEL)

nt-serial.o: nt-serial.c
	$(CC) $(CFLAGS) -c nt-serial.c

nt-parallel.o: nt-parallel.c
	$(MPICC) $(CFLAGS) -c nt-parallel.c

clean:
	rm -f $(PROGS) *.o core* *.out *.in *.txt *.csv *.png
 