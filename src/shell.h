#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/resource.h>

#define ARRAY_SIZE 1024

void read_input(void);
void parse_export(char *token, int* counter);
void parse_input(void);
void execute_command(void);
uint8_t execute_shell_builtin(void);
void write_to_log_file(void);
extern void on_child_exit(void);
void shell(void);
void setup_environment(void);

#endif /* SHELL_H */
