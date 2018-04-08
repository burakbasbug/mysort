CC=gcc
DEFS=-D_DEFAULT_SOURCE -D_BSD_SOURCE -D_SVID_SOURCE -D_POSIX_C_SOURCE=200809L
CFLAGS=-Wall -g -std=c99 -pedantic $(DEFS)

# "they are build targets, not files"
.PHONY: all clean

all: mysort

mysort: mysort.o
	$(CC) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -f *.o mysort