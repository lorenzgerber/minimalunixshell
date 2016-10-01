#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "parser.h"
#include "mish.h"
#include <sys/param.h>
#include "execute.h"


#define MAX_LENGTH 1024
#define MAX_COMMANDS 4

int main(void) {

    /*
     * Initialize variables
     */
    command comLine[MAX_COMMANDS + 1];

    /*
     * Produce Prompt
     */
    char line[MAX_LENGTH];


    while (1) {
        //sleep(1);
        fprintf(stderr, "mish %% ");
        fflush(stderr);
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
            processExternalCommands2(comLine, numberOfCommands);


        }

        /*
        printf("number of commands: %d\n", numberOfCommands);
        for(int commandLooper = 0; commandLooper < numberOfCommands; commandLooper++){
            printf("command name: %s\n", comLine[commandLooper].argv[0]);
            printf("number of args: %d\n", comLine[commandLooper].argc-1);
        }
         */


    }
    
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

    pid_t pid;
    int fd[2];

    /*
     * Loop over all commands
     */
    for(int commandIndex = 0; commandIndex < (nCommands); commandIndex++){


        // if there is a next command
        if ( commandIndex < (nCommands - 1) ){
            /*
         * create a new pipe
         */
            if(pipe(fd)){
                perror("pipe:");
                exit(EXIT_FAILURE);
            };
        }

        /*
         * spawn process
         */
        pid = fork();
        if(pid == -1){
            perror("fork:");
            exit(EXIT_FAILURE);
        }


        if(pid == 0){
            /*
             * code run in the child process
             */
            if (commandIndex != 0){
                /*
                 * Code run if this is not the first command
                 */
                fprintf(stdout, "I'm not the first command, index %d\n", commandIndex);
                // need to get the

            } else {
                /*
                 * Code Run if this is the first child
                 */


            }
            fprintf(stdout, "child command: %s\n", comLine[commandIndex].argv[0]);
            exit(0);

        } else {
            /*
             * code run in the parent process
             */

            fprintf(stdout, "Parent command index %d\n", commandIndex);


        }
    }

    /*
     * Code run after looping over all commands
     */

    return 0;
}

int processExternalCommands2(command comLine[], int nCommands){

    pid_t pid;
    int in = 0;
    int fd[2];


    /*
     * Loop over all commands
     */
    for(int commandIndex = 0; commandIndex < (nCommands -1); commandIndex++){



        if(pipe(fd)){
            perror("pipe:");
            exit(EXIT_FAILURE);
        };


        /*
         * spawn process
         */
        pid = fork();
        if(pid == -1){
            perror("fork:");
            exit(EXIT_FAILURE);
        }


        if(pid == 0){
            /*
             * code run in the child process
             */
            if (in != 0){
                dup2(in, 0);
                close(in);
            }

            if (fd[WRITE_END] != 1){
                dupPipe(fd, WRITE_END, 1);
            }

            return execvp (comLine[commandIndex].argv[0], comLine[commandIndex].argv );


        } else {

            close (fd[WRITE_END]);
            in = fd[READ_END];

        }

    }



    pid = fork();
    if(pid == -1){
        perror("fork:");
        exit(EXIT_FAILURE);
    }

    if(pid == 0){
        dup2(in, 0);
        execvp(comLine[nCommands - 1].argv[0], comLine[nCommands - 1].argv);
    }
    int status;
    waitpid(pid, &status, WUNTRACED);
    


    return 0;

}