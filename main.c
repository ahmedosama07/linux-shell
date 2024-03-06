#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<signal.h>
#include<sys/resource.h>

#define ARRAY_SIZE     100

/* Array for handling input */
char input[ARRAY_SIZE]; 
char * paresedInput[ARRAY_SIZE];

int counter = 0 , exportFlag = 0 , echoFlag = 0 , cdFlag = 0 , pwdFlag = 0;
int backGroundIndex = 0 , exitFlag = 0;

/* Function to get input from the user */
void read_input(void){
    char arr[100];
    printf("%s $ ",getcwd(arr , 100));
    scanf("%[^\n]%*c", input);
}

/* Function to clean export */
void clean_export(char * token){
    while(token != NULL){
            paresedInput[counter] = token;
            token = strtok(NULL,"=");
            counter++;
        }
        paresedInput[counter] = '\0';
        counter = 0;
}

/* Function to parse input */
void parse_input(void){
    char*token = strtok(input , " ");
    if(strcmp(token , "export") == 0){
        clean_export(token);
        exportFlag = 1;
    }
    else{
        if(strcmp(token , "cd") == 0) cdFlag = 1;
        if(strcmp(token , "echo") == 0) echoFlag = 1;
        if(strcmp(token , "pwd") == 0) pwdFlag = 1;
        if(strcmp(token , "exit") == 0) exitFlag = 1;
      while (token != NULL)
      {
          paresedInput[counter] = token;
          token = strtok(NULL," ");
          counter++;
      }   
      paresedInput[counter] = '\0' ;
      backGroundIndex = counter - 1;
      counter = 0;
    }
}

/* Function to execute built-in shell commands */
void execute_shell_bultin(void){
    if(cdFlag){
        if((paresedInput[1] == NULL) || ((strcmp(paresedInput[1],"~")==0))){
        chdir(getenv("HOME"));    
        }
        else{
            int flag = 0;
            flag = chdir(paresedInput[1]) ; 
            if( flag != 0){
                printf("Error, the directory is not found\n");
            }
        }
    }
    else if(exportFlag){
        char* data = paresedInput[2];
        /* Check for quotation marks */
        if(data[0] == '"'){
            data++;
            data[strlen(data)-1] = '\0';
            setenv(paresedInput[1] , data , 1);
        }
        else{
            /* No quotation marks */
            setenv(paresedInput[1] , paresedInput[2] , 1);
        }
    }
    else if(echoFlag){
        char*echoEnv = paresedInput[1];
        if(paresedInput[2] == NULL){
            /* There is only one command in echo */
            /* There are two cases: print variable or sentence */

            /* Remove the quotation */
            echoEnv++;
            echoEnv[strlen(echoEnv) - 1] = '\0';
            /* Case 1: print variable */
            if(echoEnv[0] == '$'){
                /* Skip dollar sign */
                echoEnv++;
                printf("%s\n",getenv(echoEnv));
            }
            else{
                /* Case 2: print sentence */
                printf("%s\n",echoEnv);
            }
        }
        else{
            char*temp = paresedInput[2];
            /* There is more than input */
            /* Remove the first quotation */
            echoEnv++;
            if(echoEnv[0] == '$'){
                echoEnv++;
                printf("%s ",getenv(echoEnv));
                /* Remove the last quotation */
                temp[strlen(temp)-1] = '\0';
                printf("%s\n",temp);
            }
            else{
                printf("%s ",echoEnv);
                /* Skip $ */
                temp++;
                /* Remove the last quotation */
                temp[strlen(temp)-1] = '\0';
                printf("%s\n",getenv(temp));
            }
        }
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
        if(paresedInput[1] == NULL){
            /* Command consists of one word */
            errorCommand = execvp(paresedInput[0] , paresedInput);
        }
        else if(paresedInput[1] != NULL){
            /* More than one word */
            /* Check if there is a variable in system environment or not */
            char* env = paresedInput[1];
            if(env[0] == '$'){
                int  i = 1;
                char*envTemp;
                env++;
                envTemp = getenv(env);
                char *exportTemp = strtok(envTemp , " ");
                while(exportTemp != NULL){
                paresedInput[i++] = exportTemp;    
                exportTemp = strtok(NULL, " ");
                }
            }
            errorCommand = execvp(paresedInput[0] , paresedInput);
        }
        if(errorCommand){
            printf("Error ! unknown command\n");
            exit(EXIT_FAILURE);
        }
    }
    else{
        /* Parent process */
        /* Foreground and background */
        if(strcmp(paresedInput[backGroundIndex] , "&")==0){
            /* We are in the background */
            /* No wait */
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
    /* Avoid zombie process */
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
    /* Same directory of the project */
    char arr[100];
    chdir(getcwd(arr , 100));
}

int main(){
   signal(SIGCHLD , on_child_exit);
   setup_environment();
   shell();
   return 0;
}
