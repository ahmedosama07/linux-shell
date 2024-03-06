# linux-shell

This is a simple shell program written in C. It provides basic functionality such as executing commands, changing directories, setting environment variables, etc.

## File Structure

- `src/shell.h`: Header file containing function declarations and constants.
- `src/shell.c`: Source file containing function definitions for the shell program.
- `src/main.c`: Source file containing the main function to run the shell program.
- `Makefile`: Makefile for compiling the shell program.
- `README.md`: This file providing information about the shell program.

## Functionality

### `replace_substring`

This function replaces all occurrences of a substring within a string with another substring.

### Input Functions

- `read_input`: Reads input from the user.
- `parse_input`: Parses the input string into individual commands and arguments.
- `parse_export`: Cleans and parses the input for the `export` command.

### Command Execution

- `execute_shell_bultin`: Executes built-in shell commands like `cd`, `echo`, `pwd`, and `export`.
- `execute_command`: Executes external shell commands using `execvp`.
- `on_child_exit`: Handles the exit of child processes to prevent zombie processes.
- `write_to_log_file`: Writes information about terminated child processes to a log file.

### Other Functions

- `setup_environment`: Sets up the environment for the shell program.
- `shell`: Main function to run the shell program in a loop.

## Usage

1. Compile the program using `make`.
2. Run the shell program by executing the `main` binary.
3. Use built-in commands like `cd`, `echo`, `pwd`, and `export`, or execute external commands.
4. Exit the shell program by typing `exit`.
