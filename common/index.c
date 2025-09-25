/* 
 * indexer.c
 *
 * This module is essentially an aliased hashtable, 
 * that stores a word and maps that to a counter
 * the counter holds a docID as a key, and value as a count
 * of how many times it shows up in the respective document
 * 
 * Eva Tate CS50, Dartmouth
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "hashtable.h"
#include "counters.h"
#include "memory.h"
#include "file.h"
#include "webpage.h"
#include <ctype.h>
#include "word.c"


//Global Types
typedef hashtable_t index_t;

//Local Functions
static void countersDelete(void *item);
static void printLine(void* file, const char* keyWord, void* counterItem);
static void printCounter(void* file, const int key, int count);



index_t* index_new(const int numSlots) {
  index_t* index;
  //create a new hashtable that is the core of the index struct
  index = hashtable_new(numSlots);

  //if memory allocation error
  if (index == NULL) {
    fprintf(stderr,"Unable to allocate memory for the index\n");
    exit(2);
  }
  return index;
}

bool index_insert(index_t* index, char* keyWord, counters_t* counter) {
  //make all lowercase
  normalizeWord(keyWord);

  //attempt to insert the key value pair into the index
  if (hashtable_insert(index,keyWord,counter)) {
    return true;
  }
  else {
    return false;
  }
}

//use hashtable iterate to go through every pair in index
void index_iterate(index_t* index, void* arg, 
			void(*itemfunc)(void* arg, const char* key, void* item)) 
{
  hashtable_iterate(index,arg,itemfunc);
  return;

}

//use hashtable delete to delete the index
void index_delete(index_t* index)
{
  //pass in counters delete to deal with freeing the memory counters occupies
	hashtable_delete(index, countersDelete);
  return;
}

//use hashtable find to see if a key exists in index
counters_t* index_find(index_t* index, const char* keyWord) {
  return hashtable_find(index,keyWord);
}

//iterates through the index and prints the values out to the file
void index_print(index_t* index, char* filename) {
  FILE* fp = fopen(filename,"w");
  if (fp==NULL) {
    fprintf(stderr,"File was unable to be opened\n");
    exit(3); //exit code for file opening error
  }
  else {
    //iterate using printline helper function which prints to a file
    //argument passed in is the file
    hashtable_iterate(index,fp,printLine);
  }
  fclose(fp);
  return;

}

//adds a specific word to the index, based on the docID and word
void index_addWord(index_t* index, char* word, int docID) {
  
  //create new counter and if the word exists, find that counter
  counters_t* counter;
  counter = index_find(index,word);
  //if the word doesn't exist, create a new exist
  if (counter == NULL) {
    counter = counters_new();
    if (counter == NULL) {
      fprintf(stderr,"Memory allocation error for new counter");
      exit(2);
    }
    //increment the specific counter based on docID
    counters_add(counter,docID);
    //insert the counter into the index
    index_insert(index,word,counter);
  }
  else {
    counters_add(counter,docID);
  }
  return;
}

//function to load a new index from an index file
index_t* index_load(char* oldFile) {
	
	FILE* fp;
	fp = fopen(oldFile,"r");
	if (fp == NULL) {
		fprintf(stderr,"Error Reading Old File/Input File");
		exit(4);
	}

  //create a new index with totalSlots based on the amount of lines in the file
  index_t* index = index_new(file_numLines(fp));

	int id;
	int num;
	char* word;
  //while there are words to read in the file
	while((word = file_readWord(fp))!=NULL) {
		counters_t* counters = counters_new();
      //scan pairs of numbers which are the docID and the count
			while (fscanf(fp, "%d %d", &id, &num) == 2) {

				//Then add them to counters and set the value
				counters_set(counters,id,num);
    }
		//insert counters into index
		index_insert(index,word,counters);
    //free the word so you can reallocate the memory
		free(word);
	}

	fclose(fp);
	return index;


}



//helper function to print the word, then call printCounter to print counter key value pair
static void printLine(void* file, const char* keyWord, void* counterItem) {
  FILE* fp = file;
  fprintf(fp,"%s",keyWord);
  counters_iterate(counterItem,fp,printCounter);
  fprintf(fp,"\n");
}

//prints the key value pairs of the counters
static void printCounter(void* file, const int key, int count)
{
	FILE *fp = file;
	fprintf(fp,  " %d %d", key, count);
}

//helper function to delete a counter, used in index_delete
static void countersDelete(void *item)
{
	if (item!=NULL) {
		counters_delete((counters_t*) item);
	}
}
