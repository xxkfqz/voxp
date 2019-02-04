CC=gcc
CFLAGS=--std=c11 -I ./include -Wall
LFLAGS=-ldl -lm -o voxp
DEBUGFLAGS=-g -O0
RELEASEFLAGS=-O3 -march=native
SRC=$(wildcard ./src/*.c)

all:
	${CC} ${CFLAGS} ${SRC} ${RELEASEFLAGS} ${LFLAGS}

debug:
	${CC} ${CFLAGS} ${SRC} ${DEBUGFLAGS} ${LFLAGS}
