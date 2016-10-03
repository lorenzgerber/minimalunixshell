#include <sys/types.h>
#include <signal.h>
#define MAX_COMMANDS 4


int kill(pid_t pid, int sig);

void sigCatcherINT( int theSignal );