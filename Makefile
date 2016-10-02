CC=gcc
CFLAGS= -Wall -std=c11 -Wextra -Werror -Wmissing-declarations -Wmissing-prototypes -Werror-implicit-function-declaration -Wreturn-type -Wparentheses -Wunused -Wold-style-definition -Wundef -Wshadow -Wstrict-prototypes -Wswitch-default -Wstrict-prototypes -Wunreachable-code

all: mish

mish: mish.o execute.o parser.o
	$(CC) $(CFLAGS) -o mish mish.o execute.o parser.o

mish.o execute.o parser.o: mish.c mish.h execute.c execute.h parser.c parser.h
	$(CC) $(CFLAGS) -c -g mish.c execute.c parser.c

clean:
	 rm -f rm mish *.o
