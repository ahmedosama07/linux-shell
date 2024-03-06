CC = gcc
CFLAGS = -Wall -Wextra

.PHONY: clean

all: main

main: main.o obj
	$(CC) $(CFLAGS) -o linux-shell.o main.o shell.o

main.o: main.c shell.h
	$(CC) $(CFLAGS) -c main.c

obj: shell.c shell.h
	$(CC) $(CFLAGS) -c shell.c

clean:
	rm -f main main.o shell.o
