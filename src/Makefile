CC=gcc
OMP=-fopenmp -DOMP
DEBUG=0
CFLAGS-SEQ=-g -O3 -Wall -DDEBUG=$(DEBUG) -std=c99 -DOMP=0
CFLAGS-OMP=-g -O3 -Wall -DDEBUG=$(DEBUG) -std=c99 -DOMP=1
LDFLAGS= -lm

CFILES = simpletests.c lockdict.c locklist.c cycletimer.c
HFILES = hdict.h locklist.h cycletimer.h

all: seq-test openmp-test

seq-test: $(CFILES) $(HFILES) 
	$(CC) $(CFLAGS-SEQ) -o seq-test $(CFILES) $(LDFLAGS)
	
openmp-test: $(CFILES) $(HFILES) 
	$(CC) $(CFLAGS-OMP) $(OMP) -o omp-test $(CFILES) $(LDFLAGS)