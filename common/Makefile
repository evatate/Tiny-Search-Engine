# Makefile for 'common' directory
#
# Eva Tate CS50 Fall 2024

OBJS = pagedir.o ../libcs50/mem.o
LIBS = -L../libcs50 -lcs50

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50
CC = gcc
MAKE = make

# the common.a library being made
LLIBS = common.a

all: $(LLIBS) pagedir.o index.o

# makes the library
$(LLIBS):	$(OBJS)
				ar rc $(LLIBS) $(OBJS)

pagedir.o:  pagedir.h ../libcs50/file.h
				$(CC) $(CFLAGS) -I../libcs50 -L../libcs50 -lcs50 -c pagedir.c 

index.o: word.h index.h
				$(CC) $(CFLAGS) -I../libcs50 -L../libcs50 -lcs50 -c index.c

.PHONY: all clean
  
clean:
		rm -f $(OBJS) $(LLIBS)
		rm -f core
		rm -rf *.dSYM  # MacOS debugger info
		rm -f *~ *.o
		rm -f indextest
		rm -f indexer
