/* 
 * indextest.c
 *
 * File that takes an indexer output files as the first parameter.
 * Uses the indexer output file to build up a new index.
 * Prints out the new index to another file 
 * which is specified by the second parameter.
 *
 * Eva Tate CS50, Dartmouth
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "hashtable.h"
#include "counters.h"
#include "index.c"
#include "file.h"


//function to load in an index from an indexer file
int main(int argc, char* argv[])
{
	// check number of arguments
	if (argc != 3) {
		fprintf(stderr, "Wrong number of arguments\n");
		exit(2);
	}
	//takes in two files as arguments
	char* oldFile = argv[1];
	char* newFile = argv[2];
	//use index_load function to take the indexer output file
	//and creates a new index with this data
	index_t* index = index_load(oldFile);
	//print out this index to the new file
	index_print(index, newFile);
	
	//delete the loaded in index to free memory
	index_delete(index);
	exit(0);
}
