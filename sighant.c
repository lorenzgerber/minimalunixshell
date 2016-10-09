/**
 * sighant.c
 * Lorenz Gerber
 * Assignment 3 5DV088 HT16
 * This file contains the implementation of the signal handling.
 */
#define _POSIX_C_SOURCE 200809L
#include "sighant.h"
#include "mish.h"

void sigCatcherINT( int theSignal ) {
    
    if ( theSignal == SIGINT) {
        for(int currentPid = 0; currentPid<MAX_COMMANDS; currentPid++){
            if (pidArray[currentPid]!=0){
                kill(pidArray[currentPid], SIGINT);
            }
        }
    }
    return;
}


Sigfunc * mySignal(int signo, Sigfunc *func) {
    struct sigaction	act, oact;

    act.sa_handler = func;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    if (signo == SIGALRM) {
#ifdef	SA_INTERRUPT
        act.sa_flags |= SA_INTERRUPT;
#endif
    } else {
#ifdef	SA_RESTART
        act.sa_flags |= SA_RESTART;
#endif
    }
    if (sigaction(signo, &act, &oact) < 0)
        return(SIG_ERR);
    return(oact.sa_handler);
}
