CC=gcc
OMP=-fopenmp -DOMP
DEBUG=0
CFLAGS-SEQ=-g -O3 -Wall -DDEBUG=$(DEBUG) -std=c99 -DOMP=0
CFLAGS-OMP=-g -O3 -Wall -DDEBUG=$(DEBUG) -std=c99 -DOMP=1
CFLAGS-RTM=-march=native -mavx -g -O3 -Wall -DDEBUG=$(DEBUG) -std=c99 -DOMP=1
LDFLAGS= -lm

CFILES-LF-LIST = lf/*.c lf/test/*.c util/cycletimer.c
HFILES-LF-LIST = lf/*.h util/cycletimer.h
CFILES-LFBACK-LIST = lfback/*.c lfback/test/*.c util/cycletimer.c
HFILES-LFBACK-LIST = lfback/*.h util/cycletimer.h

CFILES-SEQ= simpletests.c seq/seqdict.c coarse-grained-lock/coarsegrainedlist.c util/cycletimer.c util/util.c
HFILES-SEQ= hdict.h coarse-grained-lock/coarsegrainedlist.h util/cycletimer.h util/util.h

CFILES-CLOCK = simpletests.c coarse-grained-lock/*.c util/cycletimer.c util/util.c
HFILES-CLOCK = hdict.h coarse-grained-lock/*.h util/cycletimer.h util/util.h

CFILES-LOCK = simpletests.c fine-grained-lock/*.c util/cycletimer.c util/util.c
HFILES-LOCK = hdict.h fine-grained-lock/*.h util/cycletimer.h util/util.h

CFILES-HOH = simpletests.c hoh/*.c util/cycletimer.c util/util.c
HFILES-HOH = hdict.h hoh/*.h util/cycletimer.h util/util.h

CFILES-LF = simpletests.c lf/*.c util/cycletimer.c util/util.c
HFILES-LF = hdict.h lf/*.h util/cycletimer.h util/util.h

CFILES-LFBACK = simpletests.c lfback/*.c util/cycletimer.c util/util.c
HFILES-LFBACK = hdict.h lfback/*.h util/cycletimer.h util/util.h

CFILES-RTM = simpletests.c rtm/*.c util/cycletimer.c util/util.c
HFILES-RTM = hdict.h rtm/*.h util/cycletimer.h util/util.h

CFILES-MSEQ = monitor.c seq/seqdict.c coarse-grained-lock/coarsegrainedlist.c util/cycletimer.c util/util.c
HFILES-MSEQ = hdict.h coarse-grained-lock/*.h util/cycletimer.h util/util.h

CFILES-MCLOCK = monitor.c coarse-grained-lock/*.c util/cycletimer.c util/util.c
HFILES-MCLOCK = hdict.h coarse-grained-lock/coarsegrainedlist.h util/cycletimer.h util/util.h

CFILES-MLOCK = monitor.c fine-grained-lock/*.c util/cycletimer.c util/util.c
HFILES-MLOCK = hdict.h fine-grained-lock/*.h util/cycletimer.h util/util.h

CFILES-MHOH = monitor.c hoh/*.c util/cycletimer.c util/util.c
HFILES-MHOH = hdict.h hoh/*.h util/cycletimer.h util/util.h

CFILES-MLF = monitor.c lf/*.c util/cycletimer.c util/util.c
HFILES-MLF = hdict.h lf/*.h util/cycletimer.h util/util.h

CFILES-MLFBACK = monitor.c lfback/*.c util/cycletimer.c util/util.c
HFILES-MLFBACK = hdict.h lfback/*.h util/cycletimer.h util/util.h

CFILES-MRTM = monitor.c rtm/*.c util/cycletimer.c util/util.c
HFILES-MRTM = hdict.h rtm/*.h util/cycletimer.h util/util.h

all: lf-list-test lfback-list-test seq-test coarse-grained-omp-test fine-grained-omp-test hoh-omp-test lf-omp-test lfback-omp-test rtm-omp-test seq-monitor coarse-grained-monitor fine-grained-monitor hoh-monitor lf-monitor lfback-monitor rtm-monitor

all-no-rtm: lf-list-test lfback-list-test seq-test coarse-grained-omp-test fine-grained-omp-test hoh-omp-test lf-omp-test lfback-omp-test seq-monitor coarse-grained-monitor fine-grained-monitor hoh-monitor lf-monitor lfback-monitor

rtm: rtm-omp-test rtm-monitor

lf-list-test: $(CFILES-LF-LIST) $(HFILES-LF-LIST)
	$(CC) $(CFLAGS-OMP) $(OMP) -o lf-list-test $(CFILES-LF-LIST) $(LDFLAGS)

lfback-list-test: $(CFILES-LFBACK-LIST) $(HFILES-LFBACK-LIST)
	$(CC) $(CFLAGS-OMP) $(OMP) -o lfback-list-test $(CFILES-LFBACK-LIST) $(LDFLAGS)

seq-test: $(CFILES-SEQ) $(HFILES-SEQ)
	$(CC) $(CFLAGS-SEQ) -o seq-test $(CFILES-SEQ) $(LDFLAGS)

coarse-grained-omp-test: $(CFILES-CLOCK) $(HFILES-CLOCK)
	$(CC) $(CFLAGS-OMP) $(OMP) -o coarse-grained-omp-test $(CFILES-CLOCK) $(LDFLAGS)

fine-grained-omp-test: $(CFILES-LOCK) $(HFILES-LOCK)
	$(CC) $(CFLAGS-OMP) $(OMP) -o fine-grained-omp-test $(CFILES-LOCK) $(LDFLAGS)

lf-omp-test: $(CFILES-LF) $(HFILES-LF)
	$(CC) $(CFLAGS-OMP) $(OMP) -o lf-omp-test $(CFILES-LF) $(LDFLAGS)

lfback-omp-test: $(CFILES-LFBACK) $(HFILES-LFBACK)
	$(CC) $(CFLAGS-OMP) $(OMP) -o lfback-omp-test $(CFILES-LFBACK) $(LDFLAGS)

rtm-omp-test: $(CFILES-RTM) $(HFILES-RTM)
	$(CC) $(CFLAGS-RTM) $(OMP) -o rtm-omp-test $(CFILES-RTM) $(LDFLAGS)

hoh-omp-test: $(CFILES-HOH) $(HFILES-HOH)
	$(CC) $(CFLAGS-OMP) $(OMP) -o hoh-omp-test $(CFILES-HOH) $(LDFLAGS)

seq-monitor: $(CFILES-MCLOCK) $(HFILES-MCLOCK)
	$(CC) $(CFLAGS-SEQ) -o seq-monitor $(CFILES-MCLOCK) $(LDFLAGS)

coarse-grained-monitor: $(CFILES-MCLOCK) $(HFILES-MCLOCK)
	$(CC) $(CFLAGS-OMP) $(OMP) -o coarse-grained-monitor $(CFILES-MCLOCK) $(LDFLAGS)

fine-grained-monitor: $(CFILES-MLOCK) $(HFILES-MLOCK)
	$(CC) $(CFLAGS-OMP) $(OMP) -o fine-grained-monitor $(CFILES-MLOCK) $(LDFLAGS)

hoh-monitor: $(CFILES-MHOH) $(HFILES-MHOH)
	$(CC) $(CFLAGS-OMP) $(OMP) -o hoh-monitor $(CFILES-MHOH) $(LDFLAGS)

lf-monitor: $(CFILES-MLF) $(HFILES-MLF)
	$(CC) $(CFLAGS-OMP) $(OMP) -o lf-monitor $(CFILES-MLF) $(LDFLAGS)

lfback-monitor: $(CFILES-MLFBACK) $(HFILES-MLFBACK)
	$(CC) $(CFLAGS-OMP) $(OMP) -o lfback-monitor $(CFILES-MLFBACK) $(LDFLAGS)

rtm-monitor: $(CFILES-MRTM) $(HFILES-MRTM)
	$(CC) $(CFLAGS-RTM) $(OMP) -o rtm-monitor $(CFILES-MRTM) $(LDFLAGS)

clean:
	rm -f *.sh.* lf-list-test lfback-list-test seq-test coarse-grained-omp-test fine-grained-omp-test hoh-omp-test lf-omp-test lfback-omp-test rtm-omp-test coarse-grained-monitor seq-monitor fine-grained-monitor hoh-monitor lf-monitor lfback-monitor rtm-monitor
