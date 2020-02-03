/*
    Produced by Eric Floyd
    This is a basic command shell for a Linux system
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdbool.h>

void parse(char *inputLine, char **argv) {

    char line[4096];
    strcpy(line,inputLine);
    line[4096] = '\0';
    char *token = strtok(line, " \t\n");
    int i = 1;

    while (token != NULL) {
        if (strlen(token) > 100) {
            fprintf(stderr,"Error: an argument is too long.\n");
            exit(EXIT_FAILURE);
        }
        *argv++ = token;
        i++;
        token = strtok(NULL, " \t\n");
    }
    *argv = '\0';
    if (i > 50) {
        fprintf(stderr,"Error: too many arguments.\n");
        exit(EXIT_FAILURE);
    }
}

int main() {

    char *argv[4096];
    char inputLine[4096] = {0};
    int childPID;

    while (1) {
        write(1,"% ",2);
        if (fgets(inputLine, 4096, stdin) == NULL) {
            break;
        }

        if (strchr(inputLine,'\n') == NULL) {
            while (1) {
                char c = getchar();
                if (c == '\n' || c == EOF) {
                    break;
                }
            }
        }
        childPID = fork();
        if (childPID < 0) {
            fprintf(stderr,"fork error: no child process is created.\n");
            exit(EXIT_FAILURE);
        }
        if (childPID == 0) { //child process executes filename+arguments
            parse(inputLine, argv);
            execvp(*argv,argv);
            fprintf(stderr,"exec returned with error value.\n");
            exit(EXIT_FAILURE);
        } else {  //parent process waits for child process to terminate
            wait(NULL);
        }
    }
}
