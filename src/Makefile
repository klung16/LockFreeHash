CC=gcc
OMP=-fopenmp -DOMP
DEBUG=0
CFLAGS-SEQ=-g -O3 -Wall -DDEBUG=$(DEBUG) -std=c99 -DOMP=0
CFLAGS-OMP=-g -O3 -Wall -DDEBUG=$(DEBUG) -std=c99 -DOMP=1
LDFLAGS= -lm

CFILES-LOCK = simpletests.c lockdict.c locklist.c cycletimer.c
HFILES-LOCK = hdict.h locklist.h cycletimer.h
CFILES-LF = simpletests.c lfdict.c lflist.c cycletimer.c
HFILES-LF = hdict.h lflist.h cycletimer.h
CFILES-LF-LIST = lflisttest.c lflist.c cycletimer.c
HFILES-LF-LIST = lflist.h cycletimer.h
CFILES-M = monitor.c lfdict.c lflist.c cycletimer.c
HFILES-M = hdict.h lflist.h cycletimer.h
CFILES-ML = lockmonitor.c lockdict.c locklist.c cycletimer.c
HFILES-ML = hdict.h locklist.h cycletimer.h

all: lock-seq-test lock-openmp-test lf-seq-test lf-openmp-test lf-list-test monitor lock-monitor

lock-seq-test: $(CFILES-LOCK) $(HFILES-LOCK)
	$(CC) $(CFLAGS-SEQ) -o lock-seq-test $(CFILES-LOCK) $(LDFLAGS)

lock-openmp-test: $(CFILES-LOCK) $(HFILES-LOCK)
	$(CC) $(CFLAGS-OMP) $(OMP) -o lock-omp-test $(CFILES-LOCK) $(LDFLAGS)

lf-seq-test: $(CFILES-LF) $(HFILES-LF)
	$(CC) $(CFLAGS-SEQ) -o lf-seq-test $(CFILES-LF) $(LDFLAGS)

lf-openmp-test: $(CFILES-LF) $(HFILES-LF)
	$(CC) $(CFLAGS-OMP) $(OMP) -o lf-omp-test $(CFILES-LF) $(LDFLAGS)

lf-list-test: $(CFILES-LF-LIST) $(HFILES-LF-LIST)
	$(CC) $(CFLAGS-OMP) $(OMP) -o lf-list-test $(CFILES-LF-LIST) $(LDFLAGS)

monitor: $(CFILES-M) $(HFILES-M)
	$(CC) $(CFLAGS-OMP) $(OMP) -o monitor $(CFILES-M) $(LDFLAGS)

lock-monitor: $(CFILES-ML) $(HFILES-ML)
	$(CC) $(CFLAGS-OMP) $(OMP) -o lock-monitor $(CFILES-ML) $(LDFLAGS)
