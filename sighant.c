#include "sighant.h"
#include "mish.h"


int kill(pid_t pid, int sig);


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