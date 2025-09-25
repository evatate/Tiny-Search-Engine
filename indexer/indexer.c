/* 
 * indexer.c
 *
 * This file loops through the output of a crawler file, 
 * and builds up an index that maps each word to which document
 * they show up in, and how many times they show up in that document
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
#include "word.h"
#include "index.c"
#include "pagedir.h"

void indexPage(index_t* index, webpage_t* page, int docID);
void indexBuild(char* pageDirectory,char* outputFilename);


int main(int argc, char* argv[]) {
  if (argc!=3) { //are there three arguments?
    fprintf(stderr,"Incorrect number of arguments\n");
    exit(5);//command line argument error exit code
  }
  //is this a crawler directory
  if (!pagedir_validate(argv[1])) {
    fprintf(stderr,"Input directory is not a crawler directory\n");
    exit(7);
  }
  //open up the file and make it writable
  FILE* fp = fopen(argv[2],"w");
  if (fp==NULL) {
    fprintf(stderr,"File could not be found or opened to write\n");
  }
  fclose(fp);
  //build up the index with the given pageDirecotry and the output file name
  indexBuild(argv[1],argv[2]);
}

//function to build a new Index with the output of crawler
void indexBuild(char* pageDirectory, char* outputFilename) {
  //create a new index with 500 slots, anywhere between 200 and 900 is optimal
  index_t* table = index_new(500);
  int currID = 1;
  webpage_t* page;

  //load in the webpage based on the page directory and document ID
  while((page = pagedir_load(pageDirectory,currID))!=NULL) {
    //calls indexPage which goes through every word in the document
    indexPage(table,page,currID);
    //increment the ID to get the next file
    currID++;
    //free the webpage so you can reallocate
    webpage_delete(page);
  }

  //print out the index once the whole crawler directory is parsed
  index_print(table,outputFilename);
  index_delete(table);
}

//function to take a webapage, and load in every word into the index
void indexPage(index_t* index, webpage_t* page, int docID) {

  int pos = 0;
  char* word;
  //loop through every word in the webpage, and update pointer at each loop
  while ((word = webpage_getNextWord(page,&pos))!=NULL) {
    if (strlen(word)>2) { //if the word is greater than a length of two characters
      //make the world all lowercase
      normalizeWord(word);
      //add the word to the index
      index_addWord(index,word,docID);
    }
    //free the word so it is ready to be reallocated
    free(word);
  }
}
