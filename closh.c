// closh.c - COSC 315, Winter 2020
// Nate

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

#define TRUE 1
#define FALSE 0

int rc[]; //global for use with killChild()
int amount;


void killChildren(){

	for (int i = 0; i < amount; i++){
	printf("Timeout killing child %d\n", rc[i]); //prints pid 
	kill(rc[i], SIGKILL);
	}
}

// tokenize the command string into arguments - do not modify
void readCmdTokens(char* cmd, char** cmdTokens) {
    cmd[strlen(cmd) - 1] = '\0'; // drop trailing newline
    int i = 0;
    cmdTokens[i] = strtok(cmd, " "); // tokenize on spaces
    while (cmdTokens[i++] && i < sizeof(cmdTokens)) {
        cmdTokens[i] = strtok(NULL, " ");
    }
}

// read one character of input, then discard up to the newline - do not modify
char readChar() {
    char c = getchar();
    while (getchar() != '\n');
    return c;
}

// main method - program entry point
int main() {
    char cmd[81]; // array of chars (a string)
    char* cmdTokens[20]; // array of strings
    int count; // number of times to execute command
    int parallel; // whether to run in parallel or sequentially
    int timeout; // max seconds to run set of commands (parallel) or each command (sequentially)

    
    while (TRUE) { // main shell input loop
        
        // begin parsing code - do not modify
        printf("closh> ");
        fgets(cmd, sizeof(cmd), stdin);
        if (cmd[0] == '\n') continue;
        readCmdTokens(cmd, cmdTokens);
        do {
            printf("  count> ");
            count = readChar() - '0';
        } while (count <= 0 || count > 9);
        
        printf("  [p]arallel or [s]equential> ");
        parallel = (readChar() == 'p') ? TRUE : FALSE;
        do {
            printf("  timeout> ");
            timeout = readChar() - '0';
        } while (timeout < 0 || timeout > 9);
        // end parsing code
        
        
        ////////////////////////////////////////////////////////
        //                                                    //
        // TODO: use cmdTokens, count, parallel, and timeout  //
        // to implement the rest of closh                     //
        //                                                    //
        // /////////////////////////////////////////////////////
        
        // just executes the given command once - REPLACE THIS CODE WITH YOUR OWN
        //execvp(cmdTokens[0], cmdTokens); // replaces the current process with the given program

        signal(SIGALRM, killChildren); //alarm to kill children

	if (parallel) {
	amount = count; //treats rc[] as an array, and when killChildren is called it loops
	    int pid, status;
            for (int i=0; i<count; i++) {
                rc[i] = fork();
		if (rc[i]<0) exit(1); // if fork fails
                else if (rc[i] == 0) {
                    printf("pid:%d\n",(int) getpid()); //prints pid
                    execvp(cmdTokens[0], cmdTokens); //executes
                    exit(0);
                }else{			
                	//Parent will execute
                	alarm(timeout);
                }
	    }
            // Wait for all child processes to exit
            while ((pid=waitpid(-1,&status,0)) != -1);
            exit(0);
	}
	else {
		amount = 1; //treating rc[0] as just int not array, so when killchildren is called it only runs once
		for (int i=0; i<count; i++){
			rc[0] = fork(); 
		if (rc[0]<0){
			exit(1); // if fork fails
		}else if(rc[0] == 0){
			printf("Child pid:%d\n",(int) getpid()); //prints pid 
			execvp(cmdTokens[0], cmdTokens); //executes
			exit(0);
		}else{
			alarm(timeout);
			wait(NULL); //waits for process to finish before proceeding
			alarm(0); //resets alarm timer after finishes waiting

		}
		}
		exit(0);

	}
        // doesn't return unless the calling failed
        printf("Can't execute %s\n", cmdTokens[0]); // only reached if running the program failed
        exit(1);        
    }
}



