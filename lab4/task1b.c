#include "util.h"

#define SYS_READ 3
#define SYS_WRITE 4
#define SYS_OPEN 5
#define SYS_CLOSE 6
#define SYS_LSEEK 19

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

#define STDIN 0
#define STDOUT 1
#define STDERR 2

#define O_RDONLY 0
#define O_WRONLY 1
#define O_CREAT 64
#define MAX_INPUT_LENGTH 1000

extern int system_call();
void debugPrint(int, int, int, int, int);

int main (int argc , char* argv[], char* envp[])
{
    int input = STDIN;
    int output = STDOUT;
    int debugMode = 0;
    int count = 0;
    char prev = ' ';
    int i;
    for(i = 1; i < argc; i++){
        if(strncmp(argv[i], "-D", 2) == 0)
            debugMode = 1;
        else if(strncmp(argv[i], "-i", 2) == 0){
            char* filename = argv[i]+2;
            input = system_call(SYS_OPEN, filename, O_RDONLY, 0644);
        }
        else if(strncmp(argv[i], "-o", 2) == 0){
            char* filename = argv[i]+2;
            output = system_call(SYS_OPEN, filename, O_CREAT | O_WRONLY, 0644);
        }
    }
    if(debugMode){
        /* PRINTING INPUT & OUTPUT FILE PATHS */
        system_call(SYS_WRITE, STDERR, "in: ", strlen("in: "));
        system_call(SYS_WRITE, STDERR, itoa(input), strlen(itoa(input)));
        system_call(SYS_WRITE, STDERR, " ", strlen(" "));
        system_call(SYS_WRITE, STDERR, "out: ", strlen("out: "));
        system_call(SYS_WRITE, STDERR, itoa(output), strlen(itoa(output)));
        system_call(SYS_WRITE, STDERR, "\n", strlen("\n"));
    }
    while(1){
        char c;
        int ret = system_call(SYS_READ, input, &c, sizeof(char));
        if(c == '\n')
            break;
        if(c != ' ' && prev == ' '){
            count++;
        }
        prev = c;
        if(debugMode){
            debugPrint(SYS_READ, input, c, sizeof(char),ret);
        }
    }
    
    int ret = system_call(SYS_WRITE, output, itoa(count), strlen(itoa(count)));
    system_call(SYS_WRITE, output, "\n", strlen("\n"));
    
    if(debugMode){
        /* PRINTING SYSTEM CALL ARGUMENTS & RETURN CODE */
        debugPrint(SYS_WRITE, output, count, strlen(itoa(count)),ret);
    }
    return 0;
}

int countWords(char* input){
    int i;
    int count = 0;
    char prev = ' ';
    for(i = 0; i < strlen(input); i++){
        if(input[i] != ' ' && prev == ' '){
            count++;
        }
        prev = input[i];
    }
    return count;
}

void debugPrint(int arg1, int arg2, int arg3, int arg4, int ret){
     char* sysCall = "system call [arg1, arg2, arg3, arg4, ret code] = ";
     char* comma = ", ";
     system_call(SYS_WRITE, STDERR, sysCall, strlen(sysCall));
     system_call(SYS_WRITE, STDERR, itoa(arg1), strlen(itoa(arg1)));
     system_call(SYS_WRITE, STDERR, comma, strlen(comma));
     system_call(SYS_WRITE, STDERR, itoa(arg2), strlen(itoa(arg2)));
     system_call(SYS_WRITE, STDERR, comma, strlen(comma));
     system_call(SYS_WRITE, STDERR, itoa(arg3), strlen(itoa(arg3)));
     system_call(SYS_WRITE, STDERR, comma, strlen(comma));
     system_call(SYS_WRITE, STDERR, itoa(arg4), strlen(itoa(arg4)));
     system_call(SYS_WRITE, STDERR, comma, strlen(comma));
     system_call(SYS_WRITE, STDERR, itoa(ret), strlen(itoa(ret)));
     system_call(SYS_WRITE, STDERR, "\n", strlen("\n"));
}


