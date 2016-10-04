#include <sys/types.h>
#include <signal.h>
#define MAX_COMMANDS 4

typedef	void	Sigfunc(int);

int kill(pid_t pid, int sig);

void sigCatcherINT( int theSignal );

Sigfunc* mysignal(int signo, Sigfunc *func);