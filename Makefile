CC=/usr/local/Cellar/gcc/6.2.0/bin/gcc-6
CFLAGS= -Wall -std=c11 -Wextra -Werror -Wmissing-declarations -Wmissing-prototypes -Werror-implicit-function-declaration -Wreturn-type -Wparentheses -Wunused -Wold-style-definition -Wundef -Wshadow -Wstrict-prototypes -Wswitch-default -Wstrict-prototypes -Wunreachable-code

all: mish

mishx: mish.o execute.o parser.o
	$(CC) -o mish mish.o execute.o parser.o

mish.o execute.o parser.o: mish.c mish.h execute.c execute.h parser.c parser.h
	$(CC) -c -g mish.c execute.c parser.c

clean:
	 rm -f rm mish *.o
