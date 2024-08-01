CC=gcc
deps=../lib/justpos.h
LIBS=-lpanel -lncurses

justpos:
	$(CC) -Wall -o justpos $(LIBS) src/*.c
