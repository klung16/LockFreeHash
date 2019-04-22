CC=gcc

DEBUG=0
CFLAGS=-g -O3 -Wall -DDEBUG=$(DEBUG) -std=c99
LDFLAGS= -lm

CFILES = simpletests.c lockdict.c locklist.c 
HFILES = hdict.h locklist.h  

all: test

test: $(CFILES) $(HFILES) 
	$(CC) $(CFLAGS) -o test $(CFILES) $(LDFLAGS)