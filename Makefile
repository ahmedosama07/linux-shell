CC = gcc
CFLAGS = -Wall -Wextra

SRC_DIR=src
BUILD_DIR=build

.PHONY: clean

all: main

main: main.o obj
	mkdir $(BUILD_DIR)/
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/linux-shell.o main.o shell.o
	rm main.o shell.o

main.o: $(SRC_DIR)/main.c $(SRC_DIR)/shell.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/main.c

obj: $(SRC_DIR)/shell.c $(SRC_DIR)/shell.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/shell.c

clean:
	rm -r $(BUILD_DIR)/
