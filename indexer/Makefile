LIBS = ../common/common.a ../libcs50/libcs50-given.a

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../common -I../libcs50 
MAKE = make
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes

all: indexer indextest



indexer: indexer.o $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@

indextest: indextest.o $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@

indexer.o: indexer.c ../libcs50/mem.h ../common/index.h ../common/pagedir.h

indextest.o: indextest.c ../libcs50/mem.h ../common/index.h ../common/pagedir.h

valgrind: indexer
				$(VALGRIND) ./indexer data output.txt
				$(VALGRIND) ./indextest output.txt indexOutput.txt


test: 
	./testing.sh &> testing.out

clean:
	rm -f *~ *.o *.dSYM
	rm -f indexer
	rm -f indextest
	rm -f output.txt
	rm -f output1.txt
	rm -f indexOutput.txt