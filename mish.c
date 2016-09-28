#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "parser.h"
#include "mish.h"
#include "execute.h"


#define MAX_LENGTH 1024
#define MAX_COMMANDS 4

int main(int argc, char *argv[]) {

    /*
     * Initialize variables
     */
    command comLine[MAX_COMMANDS + 1];

    /*
     * Produce Prompt
     */
    char line[MAX_LENGTH];


    while (1) {
        printf("$ ");
        if (!fgets(line, MAX_LENGTH, stdin)) break;

        /*
         * Parse
         */
        int numberOfCommands = parse(line, comLine);

        /*
         * flag internal commands
         */
        if (flagInternalCommands(comLine, numberOfCommands)== -1){
            fprintf(stderr, "In the current version, it is not allowed to pipe "
                    "internal comands\n");
            exit(0);
        }

        if(isInternal(comLine)){
            /*
             * Dispatch internal commands
             */

        } else {
            /*
             * Dispatch to external commands
             */
            if(processExternalCommands(comLine, numberOfCommands)<0){
                fprintf(stderr, "External command execution failed\n");
                exit(EXIT_FAILURE);
            }

        }

        /*
        printf("number of commands: %d\n", numberOfCommands);
        for(int commandLooper = 0; commandLooper < numberOfCommands; commandLooper++){
            printf("command name: %s\n", comLine[commandLooper].argv[0]);
            printf("number of args: %d\n", comLine[commandLooper].argc-1);
        }
         */


    }


    printf("hello world\n");
    return 0;
}




/**
 * flagInternalCommands
 *
 * loops over the command list and flags internal commands. Internal command
 * names need to be defined in this function. If there is m ore than one command
 * and there is at least one internal, flagInternalCommands returns with -1 as
 * in the current version, it can not be possible to pipe internal commands.
 *
 * @param comLine
 * @param nCommands
 * @return
 */
int flagInternalCommands(command comLine[MAX_COMMANDS + 1], int nCommands ){

    int numberOfInternals = 2;
    const char *internalCommands[numberOfInternals];

    const char internal_cd[] = "cd";
    const char internal_echo[] = "echo";
    internalCommands[0] = internal_cd;
    internalCommands[1] = internal_echo;

    int anyInternal = false;

    for (int commandLooper = 0; commandLooper < nCommands; commandLooper++){
        int internal = false;
        for(int internalLooper = 0; internalLooper < numberOfInternals; internalLooper++){
            if(strcmp(comLine[commandLooper].argv[0], internalCommands[internalLooper]) == 0){
                internal = true;
                anyInternal = true;
            }
        }
        if (internal){
            comLine[commandLooper].internal = true;
        } else {
            comLine[commandLooper].internal = false;
        }
    }
    if (nCommands > 1 && anyInternal){
        return -1;
    }
    return 0;
}

/**
 *
 * isInternal
 *
 * This function returns true if there is a single command in the comLine struct array
 * and if this command has an internal flag.
 *
 * If there is a single command in the comLine array but the internal flag is false, the
 * function returns false.
 *
 * If the comLine array has several entries, the function exits.
 *
 * @param comLine
 * @param nCommands
 * @return
 */
bool isInternal (command comLine[MAX_COMMANDS + 1]){
    if (comLine[0].internal != 1){
        return false;
    }
    return true;
}


/**
 *
 * processExternalCommands
 *
 * @param comLine
 * @param nCommands
 * @return
 */
int processExternalCommands(command comLine[MAX_COMMANDS +1], int nCommands){

    for(int commandIndex = 0; commandIndex < nCommands; commandIndex++){
        if(commandIndex < nCommands-1){

            int pipe1_fd[2];
            int pipe2_fd[2];

            if(pipe(pipe1_fd)){
                perror("pipe:");
                exit(EXIT_FAILURE);
            };
            if(pipe(pipe2_fd)){
                perror("pipe2:");
                exit(EXIT_FAILURE);
            }


            pid_t pid;
            pid = fork();
            if(pid == -1){
                perror("fork:");
                exit(EXIT_FAILURE);
            }
            if(pid == 0){
                //fprintf(stdout,"test test\n");
                fprintf(stdout,"%s", comLine[commandIndex].argv[0]);
                if(commandIndex > 0){
                    // duplicate old read end to current read end
                    // close unused ones
                    dupPipe(pipe2_fd, READ_END,STDIN_FILENO);
                }

                if(commandIndex < nCommands - 1){
                    // duplicate new write end to current write end
                    // close unused one
                    dupPipe(pipe1_fd,WRITE_END,STDOUT_FILENO);
                }

                //check for redirect


                if(execvp(comLine[commandIndex].argv[0], comLine[commandIndex].argv)<0){
                    perror("execvp:");
                    exit(EXIT_FAILURE);
                }
                fprintf(stdout,"bullshit\n");

            } else {
                if(commandIndex > 0){
                    // clean up
                }
                if(commandIndex < nCommands){
                    // move file descriptor
                }
                if(nCommands > 1){
                    // close all resources
                }

            }

        }

    }


    return 0;
}
