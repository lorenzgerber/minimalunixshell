/**
 * mish.c
 * Lorenz Gerber
 * Assignment 3 5DV088 HT16
 * The main file of the mish project - a
 * minimal shell implementation.
 *
 */
#include "mish.h"
#include "sighant.h"

#define MAX_LENGTH 1024

void sigCatcherINT(int);

int pidArray[MAX_COMMANDS];

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

        mysignal(SIGINT, SIG_IGN);

        fprintf(stderr, "mish %% ");
        fflush(stderr);
        if (!fgets(line, MAX_LENGTH, stdin)){
          fprintf(stderr,"\n");
            break;
        }

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

            /*
             * cd
             */
            if(strcmp(comLine[0].argv[0],"cd")==0){
                if(chdir(comLine[0].argv[1])!=0){
                    perror("chdir:");
                }
            }
                /*
                 * echo
                 */
            else if(strcmp(comLine[0].argv[0],"echo")==0){
                for(int i = 1; i < comLine[0].argc ; i++){
                    fprintf(stdout, "%s", comLine[0].argv[i]);
                    if(i < comLine[0].argc - 1){
                        fprintf(stdout, " ");
                    };
                }
                fprintf(stdout,"\n");
            }

        } else {

            processExternalCommands(comLine, numberOfCommands);
        }
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
 * ProcessExtenalCommands
 *
 * The function that spawns new processes and executes the commands.
 * An implementation where one pipe after the other is created was
 * chosen.
 *
 * @param comLine struct from parser.h
 * @param nCommands number of commands
 * @return returns 0 if everything fine
 */
int processExternalCommands(command comLine[], int nCommands){

    pid_t pid;
    int in = 0;
    int fd[2];
    int status;



    int pidCount = 0;

    if(mysignal(SIGINT, sigCatcherINT) == SIG_ERR){
        fprintf(stderr, "Couldn't register signal handler\n");
        perror("signal");
        exit(EXIT_FAILURE);
    };




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

        pidArray[pidCount] = pid;
        pidCount++;

        if(pid == 0){

            /*
             * code run in the child process
             */
            if(commandIndex == 0 && comLine[0].infile != NULL){
                redirect(comLine[0].infile,0,READ_END);
            } else {
                close(fd[READ_END]);
            }

            if (in != 0){
                dup2(in, 0);
                close(in);
            }

            if (fd[WRITE_END] != 1){
                dupPipe(fd, WRITE_END, 1);
            }

            if(execvp (comLine[commandIndex].argv[0], comLine[commandIndex].argv )<0){
                perror("execvp");
                exit(EXIT_FAILURE);
            }

        } else {
            if (commandIndex < (nCommands - 1) && in != 0 ){
                close(in);
            }
            in = fd[READ_END];

            close (fd[WRITE_END]);
        }

    }

    pid = fork();
    if(pid == -1){
        perror("fork:");
        exit(EXIT_FAILURE);
    }

    pidArray[pidCount] = pid;

    if(pid == 0){

        if(nCommands == 1 &&  comLine[0].infile != NULL){
            redirect(comLine[0].infile,0, READ_END);
        }

        if(comLine[nCommands-1].outfile != NULL){
            redirect(comLine[nCommands-1].outfile,1,WRITE_END);
        }

        if(nCommands != 1 ){
            dupPipe(fd, READ_END, 0);
        }


        if(execvp(comLine[nCommands - 1].argv[0], comLine[nCommands - 1].argv)<0){
            perror("execvp");
            exit(EXIT_FAILURE);
        };
    }

    /*
     * wait for the status of the child processes
     */
    for(int currentPid = 0; currentPid<MAX_COMMANDS; currentPid++){
        if (pidArray[currentPid]!=0){
            waitpid(pidArray[currentPid], &status,WUNTRACED);
        }
    }
    close(fd[0]);
    close(fd[1]);

    return 0;

}



