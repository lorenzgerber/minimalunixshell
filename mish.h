#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "parser.h"

#define MAX_LENGTH 1024
#define MAX_COMMANDS 4



int flagInternalCommands(command comLine[MAX_COMMANDS + 1], int nCommands );