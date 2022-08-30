#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include "LineParser.h"
#include <linux/limits.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USER_INPUT 2048
#define TERMINATED -1
#define RUNNING 1
#define SUSPENDED 0

typedef struct process
{
    cmdLine *cmd;
    pid_t pid;
    int status;
    struct process *next;
} process;

char currentWD[PATH_MAX];
int debugMode = 0;
process **process_list;

void execute(cmdLine *);
int isDebugMode(cmdLine *);
void parentProcess(int, cmdLine *);
void childProcess(cmdLine *);
void addProcess(process **, cmdLine *, pid_t);
void printProcessList(process **);
void freeProcessList(process *);
void updateProcessList(process **);
void updateProcessStatus(process *, int, int);
int isDebugMode();
void redirectIO();
int isPipe();
int **createPipes(int);
void releasePipes(int **, int);
int *leftPipe(int **, cmdLine *);
int *rightPipe(int **, cmdLine *);
int getPipeNum(cmdLine *);
int pipeCommand();