CC=gcc
OMP=-fopenmp -DOMP
DEBUG=0
CFLAGS=-g -O3 -Wall -DDEBUG=$(DEBUG) -std=c99
LDFLAGS= -lm

CFILES = simpletests.c lockdict.c locklist.c cycletimer.c
HFILES = hdict.h locklist.h cycletimer.h

all: test openmp-test

test: $(CFILES) $(HFILES) 
	$(CC) $(CFLAGS) -o test $(CFILES) $(LDFLAGS)
	
openmp-test: $(CFILES) $(HFILES) 
	$(CC) $(CFLAGS) $(OMP) -o omp-test $(CFILES) $(LDFLAGS)