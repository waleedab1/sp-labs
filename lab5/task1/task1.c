#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include "LineParser.h"
#include <linux/limits.h>
#include <sys/types.h>
#include <sys/wait.h>


#define MAX_USER_INPUT 2048

char currentWD[PATH_MAX];
int debugMode = 0;

char* strerror();
int strcmp();
void execute(cmdLine*);
int isDebugMode(cmdLine*);
void parentProccess(int, cmdLine*);
void childProccess(cmdLine*);

int main(int argc, char **argv){

	printf("Starting the program\n");
    
    //GETTING CURRENT DIRECTORY
    getcwd(currentWD, PATH_MAX);
    
	while(1) {
        // PRINTING CURRENT DIRECTORY
        printf("~%s: ", currentWD);
        // GETTING USER INPUT
        char input[MAX_USER_INPUT];
        fgets(input, MAX_USER_INPUT, stdin);
        // PARSING USER INPUT
        cmdLine* pCmdLine = parseCmdLines(input);
        if(strcmp("quit", pCmdLine->arguments[0]) == 0)
            break;
        debugMode = isDebugMode(pCmdLine);
        // EXECUTING THE COMMAND
        if(strcmp(pCmdLine->arguments[0], "cd") == 0){
            // INCASE OF CD COMMAND WE RUN IT ON THE MAIN PROCCESS
            if(chdir(pCmdLine->arguments[1])){
                fprintf(stderr, "%s\n", strerror(errno));
            }
            getcwd(currentWD, PATH_MAX);
        }
        else{
            // IN CASE OF NORMAL COMMAND WE USE CHILD PROCCESS
            execute(pCmdLine);
        }
        
        //FREEING THE MEMRORY
        freeCmdLines(pCmdLine);
		sleep(2);
	}
	return 0;
}

void execute(cmdLine* pCmdLine){
    int pid = fork();
    if(pid == -1){
        if(debugMode){
            fprintf(stderr, "Fork failed: we are in real trouble.");
        }
    }
    else if(pid == 0)
        childProccess(pCmdLine);
    else
        parentProccess(pid, pCmdLine);
}

void childProccess(cmdLine* pCmdLine){
    int ret = execvp(pCmdLine->arguments[0], pCmdLine->arguments);
    if(ret == -1){
        perror("Error: could not execute command ");
        _exit(0377);
    }
}

void parentProccess(int pid, cmdLine* pCmdLine){
    int status;
    if(pCmdLine->blocking == 1){
        waitpid(pid, &status, 0);
    }
    char* command = pCmdLine->arguments[0];
    if(debugMode){
        fprintf(stderr, "PID: %d\n", pid);
        fprintf(stderr, "Executing command: %s\n", command);
    }
}

int isDebugMode(cmdLine* pCmdLine){
    for(int i = 0; i < pCmdLine->argCount; i++){
        if(strcmp(pCmdLine->arguments[i], "-d") == 0)
            return 1;
    }
    return 0;
}
