CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -lm

# List of executables
EXECUTABLES = make-2d print-2d stencil-2d

# Object files
OBJS = utilities.o

all: $(EXECUTABLES)

make-2d: make-2d.o $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

print-2d: print-2d.o $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

stencil-2d: stencil-2d.o $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(EXECUTABLES) *.o *.dat *.raw *.mp4

.PHONY: all clean