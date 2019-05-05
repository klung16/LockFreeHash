CC=gcc
OMP=-fopenmp -DOMP
DEBUG=0
CFLAGS-SEQ=-g -O3 -Wall -DDEBUG=$(DEBUG) -std=c99 -DOMP=0
CFLAGS-OMP=-g -O3 -Wall -DDEBUG=$(DEBUG) -std=c99 -DOMP=1
LDFLAGS= -lm

CFILES-LF-LIST = lf/*.c lf/test/*.c util/cycletimer.c
HFILES-LF-LIST = lf/*.h util/cycletimer.h
CFILES-LFBACK-LIST = lfback/*.c lfback/test/*.c util/cycletimer.c
HFILES-LFBACK-LIST = lfback/*.h util/cycletimer.h

CFILES-LOCK = simpletests.c lock/*.c util/cycletimer.c util/util.c
HFILES-LOCK = hdict.h lock/*.h util/cycletimer.h util/util.h

CFILES-LF = simpletests.c lf/*.c util/cycletimer.c util/util.c
HFILES-LF = hdict.h lf/*.h util/cycletimer.h util/util.h

CFILES-LFBACK = simpletests.c lfback/*.c util/cycletimer.c util/util.c
HFILES-LFBACK = hdict.h lfback/*.h util/cycletimer.h util/util.h

CFILES-MLOCK = monitor.c lock/*.c util/cycletimer.c util/util.c
HFILES-MLOCK = hdict.h lock/*.h util/cycletimer.h util/util.h

CFILES-MLF = monitor.c lf/*.c util/cycletimer.c util/util.c
HFILES-MLF = hdict.h lf/*.h util/cycletimer.h util/util.h

CFILES-MLFBACK = monitor.c lfback/*.c util/cycletimer.c util/util.c
HFILES-MLFBACK = hdict.h lfback/*.h util/cycletimer.h util/util.h

all: lf-list-test lfback-list-test fine-grained-omp-test lf-omp-test lfback-omp-test fine-grained-monitor lf-monitor lfback-monitor

lf-list-test: $(CFILES-LF-LIST) $(HFILES-LF-LIST)
	$(CC) $(CFLAGS-OMP) $(OMP) -o lf-list-test $(CFILES-LF-LIST) $(LDFLAGS)
	
lfback-list-test: $(CFILES-LFBACK-LIST) $(HFILES-LFBACK-LIST)
	$(CC) $(CFLAGS-OMP) $(OMP) -o lfback-list-test $(CFILES-LFBACK-LIST) $(LDFLAGS)

fine-grained-omp-test: $(CFILES-LOCK) $(HFILES-LOCK)
	$(CC) $(CFLAGS-OMP) $(OMP) -o fine-grained-omp-test $(CFILES-LOCK) $(LDFLAGS)

lf-omp-test: $(CFILES-LF) $(HFILES-LF)
	$(CC) $(CFLAGS-OMP) $(OMP) -o lf-omp-test $(CFILES-LF) $(LDFLAGS)

lfback-omp-test: $(CFILES-LFBACK) $(HFILES-LFBACK)
	$(CC) $(CFLAGS-OMP) $(OMP) -o lfback-omp-test $(CFILES-LFBACK) $(LDFLAGS)
	
fine-grained-monitor: $(CFILES-MLOCK) $(HFILES-MLOCK)
	$(CC) $(CFLAGS-OMP) $(OMP) -o fine-grained-monitor $(CFILES-MLOCK) $(LDFLAGS)
		
lf-monitor: $(CFILES-MLF) $(HFILES-MLF)
	$(CC) $(CFLAGS-OMP) $(OMP) -o lf-monitor $(CFILES-MLF) $(LDFLAGS)
	
lfback-monitor: $(CFILES-MLFBACK) $(HFILES-MLFBACK)
	$(CC) $(CFLAGS-OMP) $(OMP) -o lfback-monitor $(CFILES-MLFBACK) $(LDFLAGS)
	
clean:
	rm -f lf-list-test lfback-list-test fine-grained-omp-test lf-omp-test lfback-omp-test fine-grained-monitor lf-monitor lfback-monitor
