/* 
 * querier.c
 *
 * The TSE Querier is a standalone program that 
 * reads the index file produced by the TSE Indexer
 * and page files produced by the TSE Querier, 
 * and answers search queries submitted via stdin.
 * 
 * Look at DESIGN.md and IMPLEMENTATION.md for more information!
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

//custom struct to hold two counters, used for scoring
struct two_counters {
  counters_t* result;
  counters_t* counters;
};

//custom struct to hold a document ID and it's score based on the query
//used to sort and rank all scores after calculation is done
typedef struct {
  int docID;
  int score;
} docScorePair;

//LOCAL FUNCTIONS
bool isAndOr(char* word);
bool parseArgs(int argc, char* argv[]);
void processQuery(char* pageDirectory, index_t* index);
void printQuery(char** words, int size);
int parseInputIntoArrayAndCountWords(char* inputLine, char** words);
bool validateQuery(char** words, int size);
void countersIntersection(counters_t* counters1, counters_t* counters2); 
void intersectionHelper(void *arg, const int key, const int count);
void countersUnion(counters_t* counters1, counters_t* counters2);
void unionHelper(void* arg, const int key, const int count);
void sortScores(counters_t* scoreCounters, int numFiles, char* pageDirectory);
counters_t* calculateScore(char** words, int count, index_t* index);
int compareByValue(const void* a, const void* b);
int filesInDirectory(char* pageDirectory);
void deleteQuery(char** words, int size);


static inline int min(const int a, const int b) {
  return (a < b ? a : b);
}

//main function that parses arguments, then calls processQuery which does 
//the work of dealing with the query
//also deletes the index after query is process to free all remaining allocated memory
int main (int argc, char* argv[]) {
  //parse the arguments and check if they're valid
  if (!parseArgs(argc,argv)) {
    fprintf(stderr, "Incorrect Command Line Argument\n");
    exit(3);
  }
  //load in a new index from an index file
  index_t* index = index_load(argv[2]);
  if (index == NULL) {
    printf("Error in loading index\n");
  }

  //process the query with crawler directory and index
  processQuery(argv[1],index);

  //delete index and free all memory
  index_delete(index);

  exit(0);

}

//function to parse all the arguments, and check wether
//index is file is able to be opened,
//and directory is a valid crawler directory
bool parseArgs(int argc, char* argv[]) {
  if (argc!=3) {
    fprintf(stderr,"Incorrect number of arguments\n");
    return false;
  }

  if (!pagedir_validate(argv[1])) {
    fprintf(stderr,"Not a valid crawler directory\n");
    return false;
  }
  
  FILE* fp = fopen(argv[2],"r");
  if (fp == NULL){
    fprintf(stderr,"File is not readable\n");
    return false;
  }
  fclose(fp);
  return true;

}

//does all the work of processing the query
//starts a while loop that doesn't break till EOF
//inside while loop, it parses the query into an array
//scores the array based on the expression present,
//ranks the scores, and prints out in descending order
//which documents are the most relevant to a query
void processQuery(char* pageDirectory, index_t* index) {
  char* inputLine;
  int numWords;
  while (true) {
    printf("\nYour Query (type quit or exit if you want to end program): ");
    inputLine = file_readLine(stdin);
    if (inputLine == NULL) {
      printf("Program has been terminated \n");
      break;
    }
    //allocate memory for the the word array
    char** words = malloc(sizeof(char*)*50);
    //helper function to take the input, and turn into an array of words
    //also returns the number of words in the array including expressions
    numWords = parseInputIntoArrayAndCountWords(inputLine,words);
    
    //if there is more than one word in the array
    if (numWords!=0 && words!=NULL) {
      printQuery(words,numWords);
      //check wether the query doesn't have consecutive and/or's or at the end or beggining
      if (validateQuery(words,numWords)) {
        //initialize a new counter with all the scores of the documents based on query
        counters_t* scoreCounter = calculateScore(words,numWords,index);
        //pass in this counter to get sorted, and print out in descending order
        sortScores(scoreCounter,filesInDirectory(pageDirectory),pageDirectory);

        //free all the memory associated with the query word array
        deleteQuery(words,numWords);
        free(words);
        free(inputLine);
      }
      else {
        printf("Query is not valid\n");
      }
    }
  }
}

//print out the cleaned query without extra spaces or capital letters
void printQuery(char** words, int size) {
  printf("\nCleaned Query:");
  for (int i = 0; i<size; i++) {
    printf(" %s", words[i]);
    //free(words[i]);
  }
  printf("\n");
  return;
}

//go through each word in the words array and free the memory
void deleteQuery(char** words, int size) {
  for (int i = 0; i<size; i++) {
    free(words[i]);
  }
  return;
}

//takes a long string which represents the query, and breaks it into 
//an array of words without extra spaces
int parseInputIntoArrayAndCountWords(char* inputLine, char** words) {
  int length = strlen(inputLine);
  int count = 0;
  char currChar;
  char* tempWord;
  
  int wordLength;
  //if there are any special or erroneous characters in the query, throw out the query
  for (int i = 0; i<length; i++) {
    currChar = inputLine[i];
    if (!isalpha(currChar) && !isspace(currChar)) {
      fprintf(stderr, "There was an incorrect character in the input line \n");
      return -1;
    }
  }
  int startIndex;
  int endIndex;
  //loop through the whole input line
  for (int i = 0; i<length; i++) {
    startIndex = i;
    //find start and end index for a certain word
    while (i<length && isalpha(inputLine[i])) {
      i++;
    }
    //set index at the end of the word
    endIndex = i;

    //calculate length of word based on start and end index
    wordLength = endIndex - startIndex;
    
    //if the word is more than one character long
    if (wordLength>1) {
      tempWord = malloc(sizeof(char)*(wordLength+1));
      //copy the word to a temporary string
      strncpy(tempWord,inputLine+startIndex,wordLength);
      //insert a null terminating character
      tempWord[wordLength] = '\0';
      //add the word to the array of words and normalize it
      words[count] = normalizeWord(tempWord);
      //increment the counter that determines how many words in the array
      count++;
    }

  }
  //return the number of words in the array
  return count;
}

bool validateQuery(char** words, int size) {
  char* currWord = words[0];

  //is the first word an and or an or
  if (isAndOr(currWord)) {
    return false;
  }
  //if the array is only one word long and the first word
  //is not and or or, return true
  if (size == 1) {
    return true;
  }
  //if the last word is and or or, return false
  if (isAndOr(words[size-1])) {
    return false;
  }

  //keep track of starting word
  char* prevWord = words[0];
  //start a loop from index 1 until the second to last word
  for (int i = 1; i<size-1; i++) {
    
    currWord = words[i];
    //check if both the current word and previous word are an and/or character
    if (isAndOr(currWord) && isAndOr(prevWord)) {
      return false;
    }
    //make the previous word the current word
    prevWord = currWord;
  }
  //return true if none of these flags hit
  return true;
}

//calculates the score of each document based on the query
counters_t* calculateScore(char** words, int count, index_t* index) {
  counters_t* result = counters_new();
  counters_t* temp = NULL;
  //loop through each word in the array
  for (int i = 0; i<count; i++) {
    //if the word is a proper word, not an and/or
    if (!isAndOr(words[i])) {
      //if the temp is null
      if (temp == NULL) {
        //find the counters with respect to the word
        temp = index_find(index, words[i]);
      }
      else {
        //if temp is not null, find the intersection of existing temp
        //and the counters for the current word
        countersIntersection(temp,index_find(index,words[i]));
      }
    }
    //if the word is "or"
    else if (strcmp(words[i],"or")==0) {
      //find the union between the temp and the result
      //essentially adds up scores
      countersUnion(result,temp);
      temp = NULL;
    }
    //if the word is an and, our case above deals with it since 
    //for our use case and's are implicit
    else if (strcmp(words[i],"and")==0) {
      continue;
    }
  }
  //union the final result and temp
  countersUnion(result,temp);
  
  //return the result counter to be sorted
  return result;
  
}

//creates an array of all the docID score pairs
// and uses a qsort to sort them in descending order
void sortScores(counters_t* scoreCounters, int numFiles, char* pageDirectory) {
  //create an array of pairs of docID-scores
  docScorePair* pairs = calloc(numFiles,sizeof(docScorePair));
  int totalScores = 0;
  //loop through each document in the crawler directory
  for (int i = 0; i<numFiles; i++) {
    int score;
    //find the score at the counter for the docID
    score = counters_get(scoreCounters,i+1);

    //if there is a score that is greater than 0
    if (score>0) {
      //intialize a docScorePair
      docScorePair pair;
      //docID for this pair
      pair.docID = i+1;
      //score for this pair
      pair.score = score;
      //add the now intialized docScorePair to an array
      pairs[totalScores] = pair;
      //increment the size of the pairs array
      totalScores++;
    }
  }

  //if no results were found
  if (totalScores < 1) {
    printf("There were no results found, please search for something else \n");
  }
  else {
    //use built-in c qsort to sort the array
    //pass in custom compareByValue function which sorts by larger score first
    qsort(pairs,totalScores,sizeof(docScorePair),compareByValue);
    //loop through pairs array
    for(int i = 0; i < totalScores; i++) {
      //for each score, initalize a webpage
      webpage_t* temp = pagedir_load(pageDirectory,pairs[i].docID);
      //get the URL from this webpage
      char* url = webpage_getURL(temp);
      //print the score, then docID, then URL for the respective document
      printf("Score: %d, DocID: %d URL: %s\n", pairs[i].score,pairs[i].docID,url);
      //delete the webpage so it can be allocated as a new webpage
      webpage_delete(temp);

    }
    //delete the counter and free the memory
    counters_delete(scoreCounters);
    //deallocate the memory allocated for the pairs array
    free(pairs);
  }
}

//compares a and b, math means list will be sorted in descending order
int compareByValue(const void* a, const void* b) {
    docScorePair* pairA = (docScorePair*) a;
    docScorePair* pairB = (docScorePair*) b;
    return (pairB->score - pairA->score);
}




//Extra Helper Functions, Still Core To Functionality

//calculates the amount of files in a specified crawler directory
//meant for crawler output directories
int filesInDirectory(char* pageDirectory) {
  int docID = 0;
  if (pagedir_validate(pageDirectory)) { //make sure it is a crawler directory
    char* filename = malloc(sizeof(char)*strlen(pageDirectory)+sizeof(char)*6+1);
    FILE* fp;
    //start a while loop; will break when reached last of files
    while (true) {
      docID++; // Increment first to start checking from file 1
      //make the value of filename the pageDirectory/docID
      sprintf(filename, "%s/%d", pageDirectory, docID);
      fp = fopen(filename, "r");
      if (fp != NULL) {
        fclose(fp); // Close the file if it exists
      } 
      else {
        docID--; //Increment down one because file did not exist
        break; //break out of while loop
      }
    }
        
    free(filename); // Free the allocated memory
  }
  //-1 since we don't want the crawler file
  return docID;
}

//helper function to check string is an and/or word
bool isAndOr(char* word) {
  //check wether the string is equal to "and" or "or"
  if (strcmp(word,"and")==0 || strcmp(word,"or")==0) {
    return true;
  }
  return false;
}

//calls an iterator that iterates through a counter, and finds the intersection
void countersIntersection(counters_t* counters1, counters_t* counters2) 
{
  //initialize a new counter struct that holds two counters
  struct two_counters args = {counters1, counters2};
  //iterate through counters1 and use the intersection helper
  counters_iterate(counters1,&args,intersectionHelper);
}

//helper function to find intersection of two counters
void intersectionHelper(void* arg, const int key, const int count)
{
  struct two_counters* counterSet = arg;
  //find min value between the two counters and set that as value for first counter
  counters_set(counterSet->result, key, min(count, counters_get(counterSet->counters, key)));
}

//calls an iterator that iterates through a counter, and finds the union
void countersUnion(counters_t* counters1, counters_t* counters2) {
  //initialize a new counter struct that holds two counters
  struct two_counters args = {counters1, counters2};
  //iterate through counters2 and use the union helper
  counters_iterate(counters2,&args,unionHelper);
}

//helper function to find union of two counters
void unionHelper(void* arg, const int key, const int count)
{
  struct two_counters* counterSet = arg; 
  //adds the two values of the two counters at the respective key and adds them
  counters_set(counterSet->result,key,count + counters_get(counterSet->result,key));
}
