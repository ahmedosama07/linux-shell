CC = gcc
CFLAGS = -Wall -Wextra

SRC_DIR=src
BUILD_DIR=build

.PHONY: clean

all: main

main: main.o shell_obj string_obj
	mkdir $(BUILD_DIR)/
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/linux-shell.o main.o shell.o strings.o
	rm main.o shell.o strings.o

main.o: $(SRC_DIR)/main.c $(SRC_DIR)/shell.h $(SRC_DIR)/strings.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/main.c

shell_obj: $(SRC_DIR)/shell.c $(SRC_DIR)/shell.h 
	$(CC) $(CFLAGS) -c $(SRC_DIR)/shell.c

string_obj: $(SRC_DIR)/strings.c $(SRC_DIR)/strings.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/strings.c

clean:
	rm -r $(BUILD_DIR)/
