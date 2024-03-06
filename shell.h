#ifndef SHELL_H
#define SHELL_H

#define ARRAY_SIZE 100

void read_input(void);
void parse_export(char *token);
void parse_input(void);
void execute_shell_bultin(void);
void execute_command(void);
void write_to_log_file(void);
extern void on_child_exit(void);
void shell(void);
void setup_environment(void);

#endif /* SHELL_H */
