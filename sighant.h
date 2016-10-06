/**
 * sighant.h
 * Lorenz Gerber
 * Assignment 3 5DV088 HT16
 * Header file for the signal handling in mish.
 */
#include <sys/types.h>
#include <signal.h>

typedef	void	Sigfunc(int);

int kill(pid_t pid, int sig);

void sigCatcherINT( int theSignal );

Sigfunc* mysignal(int signo, Sigfunc *func);