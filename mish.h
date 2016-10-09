/**
 * mish.h
 * Lorenz Gerber
 * Assignment 3 5DV088 HT16
 * Header file for the main file of mish - a
 * minimal shell implementation
 */
#include <signal.h>
#include "parser.h"
#include "execute.h"
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/wait.h>


#define MAX_LENGTH 1024
#define MAX_COMMANDS 8

extern int pidArray[MAX_COMMANDS];


int flagInternalCommands(command comLine[MAX_COMMANDS + 1], int nCommands );

int processExternalCommands(command comLine[MAX_COMMANDS +1], int nCommands);

bool isInternal (command comLine[MAX_COMMANDS + 1]);
