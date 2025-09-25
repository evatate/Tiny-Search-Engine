CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../common -I../libcs50 
LIBS = ../libcs50/libcs50-given.a ../common/common.a
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes


crawler: crawler.o $(LIBS)
				$(CC) crawler.o $(LIBS) -o crawler

crawler.o: crawler.c ../libcs50/webpage.h ../common/pagedir.h ../libcs50/bag.h ../libcs50/hashtable.h 
					$(CC) $(CFLAGS) -c crawler.c

test:
			./testing.sh &> testing.out

.PHONY: test clean

valgrind: crawler
				mkdir lettersTest
				$(VALGRIND) ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html lettersTest 9

clean:
			rm -r test
			rm -r lettersTest1
			rm -r lettersTest3
			rm -r lettersTest9
			rm -rf testing.out
			rm -f core
			rm -rf *.dSYM  # MacOS debugger info
			rm -f *~ *.o
			rm -f $(LLIBS)
			rm crawler