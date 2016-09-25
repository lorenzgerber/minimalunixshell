#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "parser.h"

#define MAX_LENGTH 1024
#define MAX_COMMANDS 4
#define STRCMP(a, R, b) strcmp(a, b) R 0


int main(int argc, char *argv[]) {

    /*
     * Initialize variables
     */
    //static const unsigned int MAX_COMMANDS = 2;
    command comLine[MAX_COMMANDS + 1];

    /*
     * Produce Prompt
     */
    char line[MAX_LENGTH];


    while (1) {
        printf("$ ");
        if (!fgets(line, MAX_LENGTH, stdin)) break;
        //int expected = 1;
        int numberOfCommands = parse(line, comLine);
        printf("number of commands: %d\n", numberOfCommands);
        for(int commandLooper = 0; commandLooper < numberOfCommands; commandLooper++){
            printf("command name: %s\n", comLine[commandLooper].argv[0]);
            printf("number of args: %d\n", comLine[commandLooper].argc-1);
        }
        //assert(actual == expected);
        //system(line);
    }



    /*
     * Parse input
     */

    /*
     * Dispatch internal commands
     */

    /*
     * Dispatch to external commands
     */

    /*
     * Stop or kill current running processes
     */





    printf("hello world\n");
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