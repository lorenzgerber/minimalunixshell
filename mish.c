#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include "parser.h"
#include "mish.h"
#include <stdbool.h>

#define MAX_LENGTH 1024
#define MAX_COMMANDS 4
#define STRCMP(a, R, b) strcmp(a, b) R 0


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

        if(isInternal(comLine, numberOfCommands)){
            /*
             * Dispatch internal commands
             */

        } else {
            /*
             * Dispatch to external commands
             */


        }





        printf("number of commands: %d\n", numberOfCommands);
        for(int commandLooper = 0; commandLooper < numberOfCommands; commandLooper++){
            printf("command name: %s\n", comLine[commandLooper].argv[0]);
            printf("number of args: %d\n", comLine[commandLooper].argc-1);
        }

    }






    /*
     * Stop or kill current running processes
     */





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
bool isInternal (command comLine[MAX_COMMANDS + 1], int nCommands){
    if (nCommands > 1 ){
        fprintf(stdout, "piped command line entries can at the moment not contain internal commands\n");
        exit(0);
    } else {
        if (comLine[0].internal != 1){
            return false;
        }
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
        if(commandIndex != nCommands-1){
            int fd[2];
            if(pipe(fd)){
                perror("pipe:");
                exit(EXIT_FAILURE);
            };
        }

    }


    return 0;
}

/*
 *  below a draft of the main processing section according to the seminar
 *  
 *  for command in commandline
 *      if there is a next command
 *          pipe()
 *      fork()
 *      if child:
 *          if previous command exists:
 *              duplicate old read end to current read end
 *              close unused one
 *          if next command exists:
 *              duplicate new write end to current write end
 *              close unused one
 *          #redirects#
 *          execute command
 *      else:
 *          if previous command exists:
 *              clean up
 *          if next command exists:
 *              move file descriptor
 *          if multiple commands exist:
 *              close all resources
 *
 */