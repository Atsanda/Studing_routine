CC=gcc
CFLAGS=-c -w -Wall
SFLAGS=-fprofile-arcs -ftest-coverage 


default: tests 
 
tests: implement.o tests.o
	$(CC) $(SFLAGS) implement.o tests.o -o tests

implement.o: implement.c
	$(CC) $(CFLAGS) $(SFLAGS) implement.c -o implement.o

tests.o: tests.c
	$(CC) $(CFLAGS) $(SFLAGS) tests.c -o tests.o

clean: 
	rm -f *.o tests *.g*

.PHONY: default clean gcov
