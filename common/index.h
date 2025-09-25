#ifndef INDEX_H
#define INDEX_H

#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h" 
#include <stdio.h>
#include <stdbool.h>

typedef hashtable_t index_t;

/**************** index_new ****************/
/* creates a new index
 *
 * Caller provides:
 *  a valid integer that represents number of slots
 * We return:
 *  an index with numSlots number of slots
 *
*/
index_t* index_new(const int numSlots);


/**************** index_new ****************/
/* creates a new index
 *
 * Caller provides:
 *  an index, a string key, and counter
 * We return:
 *  a boolean that determines wether the insert works
 *
*/
bool index_insert(index_t* index, char* keyWord, counters_t* counter);


/**************** index_iterate ****************/
/* Iterates through a whole index
 *
 * Caller provides:
 *  an index, an argument, and a custom itemfunc that determines behavior
 * We return:
 *  NOTHING
 *
*/
void index_iterate(index_t* index, void* arg, 
			void(*itemfunc)(void* arg, const char* key, void* item));

/**************** index_delete ****************/
/* creates a new index
 *
 * Caller provides:
 *  an index
 * We return:
 *  NOTHING, just delete the index
 *
*/
void index_delete(index_t* index);

counters_t* index_find(index_t* index, const char* keyWord);

/**************** index_print ****************/
/* prints out index to an output file
 *
 * Caller provides:
 *  an index and a filename
 * We return:
 *  NOTHING, just print output to the specified file
 *
*/
void index_print(index_t* index, char* filename);

/**************** index_addWord ****************/
/* adds a word to the index
 *
 * Caller provides:
 *  an index, a word, and what document ID that word was found in
 * We return:
 *  NOTHING, just add the word to the index
 *
*/
void index_addWord(index_t* index, char* word, int docID);

/**************** index_load ****************/
/* creates a new index from an index file
 *
 * Caller provides:
 *  the pathname of the old file
 * We return:
 *  NOTHING
 *
*/
index_t* index_load(char* oldFile);

#endif // __INDEX_H
