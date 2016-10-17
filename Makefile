# Makefile
# Lorenz Gerber
# Assignment 3 5DV088 HT16
# Makefile for mish - minimal shell
CC=gcc
CFLAGS= -Wall -pedantic -std=c11 -Wextra -Werror -Wmissing-declarations -Wmissing-prototypes -Werror-implicit-function-declaration -Wreturn-type -Wparentheses -Wunused -Wold-style-definition -Wundef -Wshadow -Wstrict-prototypes -Wswitch-default -Wstrict-prototypes -Wunreachable-code

all: mish

mish: mish.o execute.o parser.o sighant.o
	$(CC) $(CFLAGS) -o mish sighant.o execute.o parser.o mish.o

mish.o: mish.c mish.h
	$(CC) $(CFLAGS) -c -g mish.c

execute.o: execute.c execute.h
	$(CC) $(CFLAGS) -c -g execute.c

parser.o: parser.c parser.h
	$(CC) $(CFLAGS) -c -g parser.c

sighant.o: sighant.c sighant.h
	$(CC) $(CFLAGS) -c -g sighant.c

clean:
	 rm -f rm mish *.o
