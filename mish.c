#include <stdio.h>

int main() {

    /*
     * Initialize variables
     */

    /*
     * Produce Prompt
     */

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