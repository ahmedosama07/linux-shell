#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/resource.h>
#include "shell.h"

char input[ARRAY_SIZE];
char *parsedInput[ARRAY_SIZE];

int counter = 0, exportFlag = 0, echoFlag = 0, cdFlag = 0, pwdFlag = 0;
int backGroundIndex = 0, exitFlag = 0;


/* Function to replace all occurrences of a substring with another substring */
char* replace_substring(const char *string, const char *substring, const char *replacement) {
    int length = strlen(substring);
    const char *temp = string;
    int count = 0;
    
    // Count the number of occurrences
    while ((temp = strstr(temp, substring)) != NULL) {
        temp += length;
        count++;
    }

    // Allocate memory for the new string
    int newLength = strlen(string) + (strlen(replacement) - length) * count + 1;
    char *newString = (char*)malloc(newLength);

    if (newString == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Copy the original string and perform replacements
    temp = string;
    char *current = newString;
    while (*temp) {
        if (strstr(temp, substring) == temp) {
            strcpy(current, replacement);
            temp += length;
            current += strlen(replacement);
        } else {
            *current++ = *temp++;
        }
    }
    *current = '\0';

    return newString;
}

// Function to remove preceding white spaces from a string
char* remove_leading_spaces(char* str) {
    if (str == NULL) {
        return NULL; // Return NULL if the input string is NULL
    }
    
    int len = strlen(str);
    int i, j;

    // Find the index of the first non-space character
    for (i = 0; i < len; i++) {
        if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n' && str[i] != '\r') {
            break;
        }
    }

    // Shift characters to the left to remove preceding spaces
    for (j = 0; i < len; i++, j++) {
        str[j] = str[i];
    }
    str[j] = '\0'; // Null-terminate the string at the new length

    return str;
}

// Function to remove trailing white spaces from a string
char* remove_trailing_spaces(char* str) {
    if (str == NULL) {
        return NULL; // Return NULL if the input string is NULL
    }
    
    int len = strlen(str);
    int i;

    // Find the index of the last non-space character
    for (i = len - 1; i >= 0; i--) {
        if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n' && str[i] != '\r') {
            break;
        }
    }

    // Null-terminate the string at the last non-space character
    str[i + 1] = '\0';

    return str;
}

/* Function to get input from the user */
void read_input(void){
    char cwd[1024];
    char hostname[1024];
    char username[1024];
    getlogin_r(username, sizeof(username));
    gethostname(hostname, sizeof(hostname));
    printf("%s@%s:%s $ ", username, hostname,replace_substring(getcwd(cwd , 100), getenv("HOME"), "~"));
    while (1) {
        fgets(input, sizeof(input), stdin); // Read input using fgets
        input[strcspn(input, "\n")] = '\0'; // Remove trailing newline character
        
        // Check if input is empty
        if (strlen(input) > 0) {
            break; // Exit loop if input is not empty
        }
		printf("%s@%s:%s $ ", username, hostname, replace_substring(getcwd(cwd, 100), getenv("HOME"), "~"));
    }
}


/* Function to parse export input */
void parse_export(char * token){
    while(token != NULL){
            parsedInput[counter] = token;
            token = strtok(NULL,"=");
            counter++;
        }
        parsedInput[counter] = '\0';
        counter = 0;
}

/* Function to parse input */
void parse_input(void){
    char*token = strtok(input , " ");
    if(strcmp(token , "export") == 0){
        parse_export(token);
        exportFlag = 1;
    }
    else{
        if(strcmp(token , "cd") == 0) cdFlag = 1;
        if(strcmp(token , "echo") == 0) echoFlag = 1;
        if(strcmp(token , "pwd") == 0) pwdFlag = 1;
        if(strcmp(token , "exit") == 0) exitFlag = 1;
      while (token != NULL)
      {
          parsedInput[counter] = token;
          token = strtok(NULL," ");
          counter++;
      }   
      parsedInput[counter] = '\0' ;
      backGroundIndex = counter - 1;
      counter = 0;
    }
}

/* Function to execute built-in shell commands */
void execute_shell_bultin(void){
    if(cdFlag){
        if((parsedInput[1] == NULL) || ((strcmp(parsedInput[1],"~")==0))){
        chdir(getenv("HOME"));    
        }
        else{
            int flag = 0;
            flag = chdir(parsedInput[1]) ; 
            if( flag != 0){
                printf("Error, the directory is not found\n");
            }
        }
    }
    else if(exportFlag){
        char* data = remove_leading_spaces(parsedInput[2]);
		char* env = remove_trailing_spaces(parsedInput[1]);
        /* Check for quotation marks */
        if(data[0] == '"'){
            data++;
            data[strlen(data)-1] = '\0';
			setenv(env, data , 1);
        }
        else{
            setenv(env, data , 1);
        }
    }
    else if(echoFlag){
			int i = 1;
			for(i = 1; i <= backGroundIndex; i++)
			{
				char *echoEnv = parsedInput[i];
				if(echoEnv[0] == '$'){
                echoEnv++;
                char* temp = getenv(echoEnv);
				if(temp == NULL) printf("%s ", " ");
                else printf("%s ", temp);
            }
            else{
                printf("%s ",echoEnv);
            }
			}
			printf("\n");
    }
    else if(pwdFlag){
        printf("%s\n",getcwd(NULL,0));
    }
}

/* Function to execute shell commands */
void execute_command (void){
    int status , foregroundId;
    int errorCommand = 1;
    int child_id = fork();
    if(child_id  == -1){
        printf("System Error!\n");
        exit(EXIT_FAILURE);
    }
    else if (child_id == 0){
        if(parsedInput[1] == NULL){
            errorCommand = execvp(parsedInput[0] , parsedInput);
        }
        else if(parsedInput[1] != NULL){
            char* env = parsedInput[1];
            if(env[0] == '$'){
                int  i = 1;
                char*envTemp;
                env++;
                envTemp = getenv(env);
                char *exportTemp = strtok(envTemp , " ");
                while(exportTemp != NULL){
                parsedInput[i++] = exportTemp;    
                exportTemp = strtok(NULL, " ");
                }
            }
            errorCommand = execvp(parsedInput[0] , parsedInput);
        }
        if(errorCommand){
            printf("Error ! unknown command\n");
            exit(EXIT_FAILURE);
        }
    }
    else{
        if(strcmp(parsedInput[backGroundIndex] , "&")==0){
            return;
        }
        else{
            foregroundId = waitpid(child_id , &status , 0);
            if(foregroundId == -1){
                perror("Error in waitpad function\n");
                return;
            }
        if(errorCommand){
            FILE * file = fopen("log.txt" , "a");
            fprintf(file , "%s" , "Child process terminated\n");
            fclose(file);
        }
      }
    }
}

/* Function to write to log file */
void write_to_log_file(void){
    FILE * file = fopen("log.txt" , "a");
    if(file == NULL){
        printf("Error in file\n");
        exit(EXIT_FAILURE);
    }
    else{
        fprintf(file , "%s" , "Child process terminated\n");
        fclose(file);
    }
}

/* Function to handle child exit */
void on_child_exit(){
    int status;
    pid_t id = wait(&status);
    if(id == 0 || id == -1){
        return;
    }
    else{
        write_to_log_file();
    }
}

/* Main shell function */
void shell(void){
    while(1){
        read_input();
        parse_input();
          if(exitFlag){
              exitFlag = 0;
              exit(EXIT_FAILURE);
          }
          else if(cdFlag || pwdFlag || exportFlag || echoFlag){
              execute_shell_bultin();
              cdFlag = 0;
              pwdFlag = 0;
              exportFlag = 0;
              echoFlag = 0;
        }
        else{
            execute_command();
        }
    }
}


/* Function to set up environment */
void setup_environment(void){
    char arr[100];
    chdir(getcwd(arr , 100));
}