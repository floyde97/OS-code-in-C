 /*
    Produced by Eric Floyd
    This is an example of message passing using pipes to
    communicate between Linux processes
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    int pipe1[2], pipe2[2], pipe3[2];
    int child1, child2, child3, child4;
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1 || pipe(pipe3) == -1) {
        fprintf(stderr,"Error when initializing pipes.\n");
    }

    child1 = fork();
    if (child1 == -1) { // Input Processing, child1
        fprintf(stderr,"Error forking child1.\n");
    }
    if (child1 == 0) {
        while (1) {
            char c = getchar();
            write(pipe1[1],&c,sizeof(char));
            if (c == EOF) {
                exit(0);
            }
        }
    } else {
        child2 = fork();
        if (child2 == -1) { // Newline Processing, child2
            fprintf(stderr,"Error forking child2.\n");
        }
        if (child2 == 0) {
            while (1) {
                char c;
                read(pipe1[0],&c,sizeof(char));
                if (c == '\n') {
                    char temp = ' ';
                    write(pipe2[1],&temp,sizeof(char));
                } else {
                    write(pipe2[1],&c,sizeof(char));
                }
                if (c == EOF) {
                    exit(0);
                }
            }
        } else {
            child3 = fork();
            if (child3 == -1) { // Asterisk Processing, child2
                fprintf(stderr,"Error forking child3.\n");
            }
            if (child3 == 0) {
                while (1) {
                    char c1;
                    read(pipe2[0],&c1,sizeof(char));
                    if (c1 == '*') {
                        char c2;
                        read(pipe2[0],&c2,sizeof(char));
                        if (c2 == '*') {
                            char temp = '^';
                            write(pipe3[1],&temp,sizeof(char));
                        } else {
                            write(pipe3[1],&c1,sizeof(char));
                            write(pipe3[1],&c2,sizeof(char));
                            if (c2 == EOF) {
                                exit(0);
                            }
                        }
                    } else {
                        write(pipe3[1],&c1,sizeof(char));
                        if (c1 == EOF) {
                            exit(0);
                        }
                    }
                }
            } else {
                child4 = fork();
                if (child4 == -1) { // Output Processing, child2
                    fprintf(stderr,"Error forking child4.\n");
                }
                if (child4 == 0) {
                    char outputArray[80];
                    int count = 0;
                    while (1) {
                        if (count == 80) {
                            for (int i=0; i<80; i++) {
                                printf("%c", outputArray[i]);
                            }
                            printf("\n");
                            count = 0;
                        } else {
                            char c;
                            read(pipe3[0],&c,sizeof(char));
                            if (c == EOF) {
                                exit(0);
                            } else {
                                outputArray[count++] = c;
                            }
                        }
                    }
                } else {
                    wait(NULL);
                }
                wait(NULL);
            }
            wait(NULL);
        }
        wait(NULL);
    }
    return 0;
}
