CC = gcc
CFLAGS = -c -g -Wall -Wextra
LFLAGS = -g -Wall -Wextra

INPUTFILESIZEMEGABYTES = 1

KILO = 1024
MEGA = $(shell echo $(KILO)\*$(KILO) | bc)
INPUTFILESIZEBYTES = $(shell echo $(MEGA)\*$(INPUTFILESIZEMEGABYTES) | bc)
INPUTBLOCKSIZEBYTES = $(KILO)
INPUTBLOCKS = $(shell echo $(INPUTFILESIZEBYTES)\/$(INPUTBLOCKSIZEBYTES) | bc)

.PHONY: all clean

all: pi pi-sched rw rr_quantum cpu_intensive io_intensive mixed non_intensive

pi: pi.o
	$(CC) $(LFLAGS) $^ -o $@ -lm

pi-sched: pi-sched.o
	$(CC) $(LFLAGS) $^ -o $@ -lm

cpu_intensive: cpu_intensive.o
	$(CC) $(LFLAGS) $^ -o $@ -lm

io_intensive: io_intensive.o
	$(CC) $(LFLAGS) $^ -o $@ -lm

non_intensive: non_intensive.o
	$(CC) $(LFLAGS) $^ -o $@ -lm

mixed: mixed.o
	$(CC) $(LFLAGS) $^ -o $@ -lm

rw: rw.o rwinput
	$(CC) $(LFLAGS) rw.o -o $@ -lm

rr_quantum: rr_quantum.o
	$(CC) $(LFLAGS) $^ -o $@ -lm

pi.o: pi.c
	$(CC) $(CFLAGS) $<

pi-sched.o: pi-sched.c
	$(CC) $(CFLAGS) $<

cpu_intensive.o: cpu_intensive.c
	$(CC) $(CFLAGS) $<

io_intensive.o: io_intensive.c
	$(CC) $(CFLAGS) $<

non_intensive.o: non_intensive.c
	$(CC) $(CFLAGS) $<

mixed.o: mixed.c
	$(CC) $(CFLAGS) $<

rw.o: rw.c
	$(CC) $(CFLAGS) $<

rwinput: Makefile
	dd if=/dev/urandom of=./rwinput bs=$(INPUTBLOCKSIZEBYTES) count=$(INPUTBLOCKS)

rr_quantum.o: rr_quantum.c
	$(CC) $(CFLAGS) $<

clean: testclean
	rm -f pi pi-sched rw rr_quantum cpu_intensive io_intensive mixed non_intensive
	rm -f rwinput
	rm -f *.o
	rm -f *~
	rm -f handout/*~
	rm -f handout/*.log
	rm -f handout/*.aux

testclean:
	rm -f output/rwoutput*
	rm -f output/mixedrwoutput*
