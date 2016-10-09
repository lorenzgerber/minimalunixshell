/**
 * execute.c
 * Lorenz Gerber
 * Assignment 3 5DV088 HT16
 *
 * This file contains the functions dupPipe and redirect which are
 * used with streams. dupPipe is a combination of dup2 and close while
 * redirect uses just dup2.
 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "execute.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>


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
 * 			or writing. 0 -> redirect input from file, 1 -> redirect
 * 			output to file.
 * 		destfd	the standard I/O file descriptor which shall be
 *			redirected
 * Returns:	-1 on error, else destfd
 */
int redirect(char *filename, int flags, int destfd){


    if (flags==0){
        int fdr = open (filename, O_RDONLY);
        if (fdr<0){
            perror(filename);
        } else {
            if(dup2(fdr, destfd) < 0){
                perror("dup2");
            };
        }






    } else {

        if(access(filename, W_OK)==0){
            errno = EEXIST;
            perror("write redirect");
        } else {
            int fdw = open(filename, O_WRONLY | O_CREAT | O_EXCL, S_IRWXU );
            if (fdw<0){
                perror(filename);
            }

            if(dup2(fdw, destfd) < 0){
                perror("dup2");
            }
        }


    }


    return 0;
}
