CC=gcc

all: mysort

mysort: mysort.o
	$(CC) $^ -o $@

%.o: %.c
	$(CC) -std=c99 -pedantic -Wall -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_SVID_SOURCE -D_POSIX_C_SOURCE=200809L -g -c $^ -o $@

clean:
	rm -f mysort
	rm -f *.o