#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int strlen();


int main(int argc, char **argv){
    int pipefd[2];
	pid_t pid;
    char buf;
    char* msg = "Hello\n";
    
    if (pipe(pipefd) == -1){
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    
    pid = fork();
    
    if(pid == -1){
        perror("fork");
        exit(EXIT_FAILURE); 
    }
    
    if(pid == 0){
        // CLOSE THE READ-END
        close(pipefd[0]);
        // WRITE HELLO TO THE WRITE-END
        write(pipefd[1], msg, strlen(msg));
        // CLOSE THE WRTIE-END
        close(pipefd[1]);
        // EXIT
        exit(EXIT_SUCCESS);
    }
    else{
        // CLOSE THE WRITE-END
        close(pipefd[1]);
        // READ FROM READ-END CHAR BY CHAR
        while (read(pipefd[0], &buf, 1) > 0)
            write(STDOUT_FILENO, &buf, 1);
        // CLOSE THE READ-END
        close(pipefd[0]);
        // WAIT FOR CHILD PROCESS TO END
        wait(NULL);
        // EXIT
        _exit(EXIT_SUCCESS);
    }
	return 0;
}
