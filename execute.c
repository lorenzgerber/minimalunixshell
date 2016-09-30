#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "execute.h"


/* Duplicate a pipe to a standard I/O file descriptor, and close both pipe ends
 * Arguments:	pip	the pipe
 *		end	tells which end of the pipe shold be dup'ed; it can be
 *			one of READ_END or WRITE_END
 *		destfd	the standard I/O file descriptor to be replaced
 * Returns:	-1 on error, else destfd
 */
int dupPipe(int pip[2], int end, int destfd){
    if(dup2(pip[end],destfd)<0){
        perror("dup2:");
        exit(EXIT_FAILURE);
    }

    if(close(pip[end])<0){
        perror("close:");
        exit(EXIT_FAILURE);
    };
    return 0;
}

/* Redirect a standard I/O file descriptor to a file
 * Arguments:	filename	the file to/from which the standard I/O file
 * 				descriptor should be redirected
 * 		flags	indicates whether the file should be opened for reading
 * 			or writing
 * 		destfd	the standard I/O file descriptor which shall be
 *			redirected
 * Returns:	-1 on error, else destfd
 */
int redirect(char *filename, int flags, int destfd){
    char *test = filename;
    int testflags = flags;
    int testdestfd  = destfd;
    printf("%s %d %d", test, testflags, testdestfd);

    return 0;
}