CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99 -g

gps: gps.o splitter.o auxillary.o main.o
	$(CC) $(CFLAGS) -o gps gps.o splitter.o auxillary.o main.o

gps.o: gps.c splitter.h auxillary.h
	$(CC) $(CFLAGS) -c gps.c

splitter.o: splitter.c splitter.h auxillary.h
	$(CC) $(CFLAGS) -c splitter.c

auxillary.o: auxillary.c auxillary.h
	$(CC) $(CFLAGS) -c auxillary.c

main.o: main.c gps.h splitter.h
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f *.o
