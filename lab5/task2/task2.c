#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include "LineParser.h"
#include <linux/limits.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>


#define MAX_USER_INPUT 2048
#define TERMINATED -1
#define RUNNING 1
#define SUSPENDED 0

typedef struct process {
    cmdLine* cmd;
    pid_t pid;
    int status;
    struct process* next;
} process;

char currentWD[PATH_MAX];
int debugMode = 0;
process** process_list;

char* strerror();
int strcmp();
void execute(cmdLine*);
int isDebugMode(cmdLine*);
void parentProcess(int, cmdLine*);
void childProcess(cmdLine*);
void addProcess(process**, cmdLine*, pid_t);
void printProcessList(process**);
void freeProcessList(process*);
void updateProcessList(process**);
void updateProcessStatus(process*, int, int);


int main(int argc, char **argv){
	printf("Starting the program\n");
    
    //GETTING CURRENT DIRECTORY
    getcwd(currentWD, PATH_MAX);
    process_list = malloc(sizeof(process));
    
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
            // INCASE OF CD COMMAND WE RUN IT ON THE MAIN PROCESS
            if(chdir(pCmdLine->arguments[1])){
                fprintf(stderr, "%s\n", strerror(errno));
            }
            getcwd(currentWD, PATH_MAX);
        }
        else if (strcmp(pCmdLine->arguments[0], "showprocs") == 0){
            printProcessList(process_list);
        }
        else if (strcmp(pCmdLine->arguments[0], "nap") == 0){
            int t = atoi(pCmdLine->arguments[1]);
            int pid = atoi(pCmdLine->arguments[2]);
            pid_t curr = fork();
            if(curr == 0){
                kill(pid, SIGTSTP);
                sleep(t);
                kill(pid, SIGCONT);
                exit(0);
            }
        }
        else if (strcmp(pCmdLine->arguments[0], "stop") == 0){
            int pid = atoi(pCmdLine->arguments[1]);
            pid_t curr = fork();
            if(curr == 0){
                kill(pid, SIGINT);
                exit(0);
            }
            
        }
        else{
            // IN CASE OF NORMAL COMMAND WE USE CHILD PROCESS
            execute(pCmdLine);
        }
        //sleep(2);
	}
    //FREEING THE MEMRORY
    freeProcessList(process_list[0]);
    free(process_list);
	return 0;
}

void execute(cmdLine* pCmdLine){
    pid_t pid = fork();
    
    addProcess(process_list, pCmdLine, pid);
    
    if(pid == -1){
        if(debugMode){
            fprintf(stderr, "Fork failed: we are in real trouble.");
        }
    }
    else if(pid == 0)
        childProcess(pCmdLine);
    else
        parentProcess(pid, pCmdLine);
}


void childProcess(cmdLine* pCmdLine){
    int ret = execvp(pCmdLine->arguments[0], pCmdLine->arguments);
    if(ret == -1){
        perror("Error: could not execute command ");
        _exit(0377);
    }
}


void parentProcess(pid_t pid, cmdLine* pCmdLine){
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

void addProcess(process** process_list, cmdLine* cmd, pid_t pid){
    // ADDS A NEW PROCESS TO THE ENDOF THE LIST
    process* new_process = (process*) (malloc(sizeof(process)));
    new_process->cmd = cmd;
    new_process->pid = pid;
    new_process->status = RUNNING;
    new_process->next = NULL;
    process* current = process_list[0];
    if(current == NULL){
        process_list[0] = new_process;
    }
    else{
        while(current->next != NULL){
            current = current->next;
        }
        current->next = new_process;
    }
}

void printProcessList(process** process_list){
    updateProcessList(process_list);
    process* current = process_list[0];
    process* prev = NULL;
    printf("ID \t COMMAND \t STATUS\n");
    while(current != NULL){
        pid_t pid = current->pid;
        char* command = current->cmd->arguments[0];
        int status = current->status;
        printf("%d \t %s \t %s\n", pid, command, status == RUNNING ? "Running" : status == SUSPENDED ? "Suspended" : "Terminated");
        if(current->status == TERMINATED){
            if(prev == NULL){
                freeCmdLines(current->cmd);
                process* prevProcess = current;
                *process_list = current->next;
                free(prevProcess);
            }
            else{
                freeCmdLines(current->cmd);
                process* prevProcess = current;
                prev->next = current->next;
                free(prevProcess);
                current = prev;
            }
        }
        prev = current;
        current = current->next;
    }
}

void freeProcessList(process* process_list){
    process* current  = process_list;
    while(current != NULL){
        freeCmdLines(current->cmd);
        process* prev = current;
        current = current->next;
        free(prev);
    }
}

void updateProcessList(process** process_list){
    process* current = process_list[0];
    while(current != NULL){
        int status = current->status;
        int pid = current->pid;
        if(waitpid(pid, &status, WNOHANG | WCONTINUED | WUNTRACED) != 0){
            if(WIFCONTINUED(status)){
                updateProcessStatus(current, pid, RUNNING);
            }
            else if(WIFEXITED(status) | WIFSIGNALED(status)){
                updateProcessStatus(current, pid, TERMINATED);
            }
            else if(WIFSTOPPED(status)){
                updateProcessStatus(current, pid, SUSPENDED);
            }
        }
        current = current->next;
    }
}

void updateProcessStatus(process* process, int pid, int status){
    process->status = status;
}
