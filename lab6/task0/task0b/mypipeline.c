#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include "LineParser.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int strlen(char*);
int isDebugMode(char**, int);
int strcmp();
int isDebugMode();


int main(int argc, char **argv){
    int pipefd[2];
	pid_t pid1;
    pid_t pid2;
    int debugMode = isDebugMode(argv, argc);
    cmdLine* cmd1 = parseCmdLines("ls -l");
    cmdLine* cmd2 = parseCmdLines("tail -n 2");
    
    
    if(debugMode){
        fprintf(stderr, "(parent_process>forking...)\n");
    }
    
    if (pipe(pipefd) == -1){
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    
    pid1 = fork();
    
    if(debugMode && pid1 != 0){
        fprintf(stderr, "(parent_process>created process with id: %d)\n", pid1);
    }
    
    if(pid1 == -1){
        perror("fork");
        exit(EXIT_FAILURE); 
    }
    
    if(pid1 == 0){
        // CLOSE THE STANDARD OUTPUT
        close(1);
        // DUBLICATING WRITE-END
        dup(pipefd[1]);
        // CLOSE THE STANDARD OUTPUT
        close(pipefd[1]);
        // EXECUTING COMMAND
        execvp(cmd1->arguments[0], cmd1->arguments);
        // EXIT
        exit(EXIT_SUCCESS);
    }
    else{
        // CLOSE THE WRITE-END
        close(pipefd[1]);
    }
    
    // FORK A CHILD PROCESS
    pid2 = fork();
        
    if(pid2 == -1){
        perror("fork");
        exit(EXIT_FAILURE);
    }
    
    if(pid2 == 0){
        // CLOSE THE STANDARD INPUT
        close(0);
        // DUBLICATING READ-END
        dup(pipefd[0]);
        // CLOSE THE STANDARD OUTPUT
        close(pipefd[0]);
        // EXECUTING COMMAND
        execvp(cmd2->arguments[0], cmd2->arguments);
        // EXIT
        exit(EXIT_SUCCESS); 
    }
    else{
        // CLOSE THE READ-END
        close(pipefd[0]);
        // WAIT FOR CHILD PROCESS
        waitpid(pid1, NULL, 0);
        // WAIT FOR CHILD PROCESS
        waitpid(pid2, NULL, 0);
        // EXIT
        exit(EXIT_SUCCESS);
    }
	return 0;
}


int isDebugMode(char** argv, int argc){
    for(int i = 0; i < argc; i++){
        if(strcmp(argv[i], "-d") == 0)
            return 1;
    }
    return 0;
}
