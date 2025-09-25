LIBS = ../common/common.a ../libcs50/libcs50-given.a

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../common -I../libcs50 
MAKE = make
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes

all: querier fuzzquery



fuzzquery: fuzzquery.o $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@

querier: querier.o $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@


querier.o: querier.c ../libcs50/mem.h ../common/index.h ../common/pagedir.h
fuzzquery.o: fuzzquery.c ../libcs50/mem.h

valgrind: querier
				$(VALGRIND) ./querier ../indexer/data1 ../indexer/output1.txt

query: querier
			./querier ~/cs50-dev/shared/tse/crawldata/wikipedia_2 ~/cs50-dev/shared/tse/output/wikipedia-2.index


test: 
		bash -v testing.sh &> testing.out

clean:
	rm -f *~ *.o *.dSYM
	rm -f querier
	rm -f querier.o
	rm -f fuzzquery.o
	rm -f fuzzquery
	rm -f testing.out