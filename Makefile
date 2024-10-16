CC=gcc
deps=../lib/justpos.h
LIBS=-lpanel -lncursesw

justpos:
	$(CC) -Wall -o justpos $(LIBS) src/*.c
