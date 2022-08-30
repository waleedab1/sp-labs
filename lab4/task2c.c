#include "util.h"

#define SYS_READ 3
#define SYS_WRITE 4
#define SYS_OPEN 5
#define SYS_CLOSE 6
#define SYS_LSEEK 19
#define SYS_GETDENTS 141

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

#define STDIN 0
#define STDOUT 1
#define STDERR 2

#define O_RDONLY 0
#define O_WRONLY 1
#define O_CREAT 64

#define DT_UNKNOWN 0
#define DT_FIFO 1
#define DT_CHR 2
#define DT_DIR 4
#define DT_BLK 6
#define DT_REG 8
#define DT_LNK 10
#define DT_SOCK 12

typedef struct ent {
    int inode;
    int offset;
    short len;
    char name[];
} ent;

extern int system_call();
extern void infection(int);
extern void infector(char*);
void debugPrint(int, int, char*, int, int);
void debugPrintLengthAndType(int, char*);
char* getEntType(char);
void printNameAndType(char*, char*);

int main (int argc , char* argv[], char* envp[])
{
    char* comment = "Flame 2 strikes!";
    int debugMode = 0;
    int prefixMode = 0;
    int afixMode = 0;
    int sufixMode = 0;
    char* prefix = "";
    char* afix = "";
    char* sufix = "";
    char buffer[8192];
    int bytesRead;
    int dir;
    int ret;
    int i;
        
    for(i = 1; i < argc; i++){
        if(strncmp(argv[i], "-D", 2) == 0)
            debugMode = 1;
        if(strncmp(argv[i], "-p", 2) == 0){
            prefixMode = 1;
            prefix = argv[i]+2;
        }
        if(strncmp(argv[i], "-s", 2) == 0){
            sufixMode = 1;
            sufix = argv[i]+2;
        }
        if(strncmp(argv[i], "-a", 2) == 0){
            afixMode = 1;
            afix = argv[i]+2;
        }
    }
    
    /* WRITTING THE COMMENT */
    ret = system_call(SYS_WRITE, STDOUT, comment, strlen(comment));
    system_call(SYS_WRITE, STDOUT, "\n", strlen("\n"));
    
    if(debugMode){
        debugPrint(SYS_WRITE, STDOUT, comment, strlen(comment), ret);
        system_call(SYS_WRITE, STDOUT, "\n", strlen("\n"));
    }
    
    /* OPENING THE DIRECTORY & GETING THE ENTRIES */
    dir = system_call(SYS_OPEN, ".", O_RDONLY, 0644);
    bytesRead = system_call(SYS_GETDENTS, dir, buffer, 8192);
    
    /* PRINTING THE NAMES OF THE ENTRIES */
    i = 0;
    while(i < bytesRead){
        ent* entp = (ent*)(buffer + i);
        char entType = *(buffer+i+entp->len-1);
        char* type = getEntType(entType);
        
        /* PRINT FILES WITH PREFIX */
        if(prefixMode){
            if(strncmp(prefix, entp->name, strlen(prefix)) == 0){
                printNameAndType(entp->name, type);
            }
        }
        else if(sufixMode){
            int length = strlen(entp->name) - strlen(sufix);
            if(strncmp(sufix, entp->name + length, strlen(sufix)) == 0){
                printNameAndType(entp->name, type);
            }
        }
        else if(afixMode){
            if(strncmp(afix, entp->name, strlen(afix)) == 0){
                infector(entp->name);
            }
        }
        else{
            system_call(SYS_WRITE, STDOUT, "name: ", strlen("name: "));
            system_call(SYS_WRITE, STDOUT, entp->name, strlen(entp->name));
            system_call(SYS_WRITE, STDOUT, "\n", strlen("\n"));
        }
        /* DEBUG PRINTS */
        if(debugMode){
            if(!prefixMode || (prefixMode && strncmp(prefix, entp->name, strlen(prefix)) == 0)){
            debugPrint(SYS_WRITE, STDOUT, entp->name, strlen(entp->name), ret);
            debugPrintLengthAndType(entp->len, type);
            }
        }
        i = i + entp->len;
    }
    system_call(SYS_CLOSE, dir);
    return 0;
}


void debugPrint(int arg1, int arg2, char* arg3, int arg4, int ret){
     char* sysCall = "DEBUG: system call [arg1, arg2, arg3, arg4, ret code] = ";
     char* comma = ", ";
     system_call(SYS_WRITE, STDERR, sysCall, strlen(sysCall));
     system_call(SYS_WRITE, STDERR, itoa(arg1), strlen(itoa(arg1)));
     system_call(SYS_WRITE, STDERR, comma, strlen(comma));
     system_call(SYS_WRITE, STDERR, itoa(arg2), strlen(itoa(arg2)));
     system_call(SYS_WRITE, STDERR, comma, strlen(comma));
     system_call(SYS_WRITE, STDERR, arg3, strlen(arg3));
     system_call(SYS_WRITE, STDERR, comma, strlen(comma));
     system_call(SYS_WRITE, STDERR, itoa(arg4), strlen(itoa(arg4)));
     system_call(SYS_WRITE, STDERR, comma, strlen(comma));
     system_call(SYS_WRITE, STDERR, itoa(ret), strlen(itoa(ret)));
     system_call(SYS_WRITE, STDERR, "\n", strlen("\n"));
}

void debugPrintLengthAndType(int len, char* type){
     system_call(SYS_WRITE, STDERR, "DEBUG: Length: ", strlen("DEBUG: Length: "));
     system_call(SYS_WRITE, STDERR, itoa(len), strlen(itoa(len)));
     system_call(SYS_WRITE, STDERR, " ", strlen(" "));
     system_call(SYS_WRITE, STDERR, "Type: ", strlen("Type: "));
     system_call(SYS_WRITE, STDERR, type, strlen(type));
     system_call(SYS_WRITE, STDERR, "\n\n", strlen("\n\n"));
}

void printNameAndType(char* name, char* type){
     system_call(SYS_WRITE, STDERR, "name: ", strlen("name: "));
     system_call(SYS_WRITE, STDERR, name, strlen(name));
     system_call(SYS_WRITE, STDERR, " ", strlen(" "));
     system_call(SYS_WRITE, STDERR, "Type: ", strlen("Type: "));
     system_call(SYS_WRITE, STDERR, type, strlen(type));
     system_call(SYS_WRITE, STDERR, "\n", strlen("\n"));
}

char* getEntType(char entType){
    switch(entType){
            case DT_BLK:
                return "Block device";
                break;
            case DT_CHR:
                return "Char device";
                break;
            case DT_DIR:
                return "Directory";
                break;
            case DT_FIFO:
                return "FIFO";
                break;
            case DT_LNK:
                return "Symlink";
                break;
           case DT_REG:
                return "Regular";
                break;
            case DT_SOCK:
                return "Socket";
                break;
            case DT_UNKNOWN:
                return "Unknown";
                break;
    }
    return "";
}

