/* 
 * crawler.c
 *
 * This file crawls through a seed webpage
 * finds all the URLs in that webpage
 * keeps on crawling and finding URLs until maxDepth is reached
 *
 * Eva Tate CS50, Dartmouth
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "webpage.h"
#include "hashtable.h"
#include "bag.h"
#include "memory.h"
#include "../common/pagedir.h"



int main(const int argc, char* argv[]);
static void parseArgs(const int argc, char* argv[],
                    char** seedURL, char** pageDirectory, int* maxDepth);
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth);
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);

//runs parseArgs and crawl functions
int main(const int argc, char* argv[]) {
  if (argc!=4) {
    fprintf(stderr, "Number of arguments were incorrect\n");
    return 1;
  }
  
  char* seedURL = argv[1];
  char* pageDirectory = argv[2];
  int maxDepth = atoi(argv[3]);
  

  //call the parse args function that parses all arguments and error checks
  parseArgs(argc,argv,&seedURL,&pageDirectory,&maxDepth);

  //beggining crawling starting from the seedURl
  //puts results in the correct directory
  //crawls until it reaches maxDepth
  crawl(seedURL,pageDirectory,maxDepth);

}

//function to parse all the arguments and 
//see if they are correctly passed from the command line
static void parseArgs(const int argc, char* argv[],
                      char** seedURL, char** pageDirectory, int* maxDepth)
{
  *seedURL = normalizeURL(argv[1]);
  
  //intially the .crawler file inside the correct directory
  if (!pagedir_init(*pageDirectory)) {
    fprintf(stderr, "Page directory could not be initialized.\n");
    exit(2);
  }

  //check if the seedURL is an internal URL
  if (!isInternalURL(*seedURL)) {
    fprintf(stderr, "Seed URL is not an internal URL.\n");
    exit(3);
  }
  //check wether the maxDepth is within the correct bounds of [0,10]
  if(*maxDepth <= 0 && *maxDepth >= 10) {
    fprintf(stderr, "Max depth is either above 10, or below 0.\n");
    exit(4);
  }
  return;
}

//crawler function that starts at seedURL, and outputs a set of documents
//these documents contain html url's and depth of that file
//keeps on crawling till it reaches maxDepth, utilizing a dfs algorithim
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth) {

	bag_t* bag = bag_new();

  hashtable_t* ht = hashtable_new(200);

  int currentDepth = 0;

  //inser the seed URL into the hashtable
  hashtable_insert(ht,seedURL,"z");
  //create a new wepbage based on the seedURL 
  //to be inserted into the bag as the first part of the dfs
  webpage_t* seedPage = webpage_new(seedURL,currentDepth,NULL);
  if(seedPage==NULL) {//memory allocation check
    fprintf(stderr, "Memory Allocation Error for the seedPage\n");
    exit(1);
  }

  //insert the page with the seedURL into the bag
  bag_insert(bag,seedPage);
  
  //create a docID counter that increments everytime you create a new doc
  int docID = 1;
  
  webpage_t* nextPage;
  //find out how to check if bag is not empty, then extract inside that if statement
  while((nextPage = bag_extract(bag)) != NULL) {
    
    if (webpage_fetch(nextPage)) { //fetch the nextPage and see if it exists

      //save the contents of the webpage to a new document with a unique docID
      pagedir_save(nextPage,pageDirectory,docID); 
      docID++;

      //if the current depth is less than the specified maxDepth value, continue
      if (webpage_getDepth(nextPage)<maxDepth) {
        //scan the page for more URLs that will be inserted into the bag
				pageScan(nextPage,bag,ht);
      }
      
    }
    else {
      //print out which webpage was unable to be fetched
      char* newString = webpage_getURL(nextPage);
      fprintf(stderr,"URL: %s\n",newString);
      fprintf(stderr, "Failed to fetch webpage:\n");
      continue; // Skip 
    }
    
    //delete the nextPage webpage
    webpage_delete(nextPage);
    
    
  }
  //delete the hashtable and bag
  hashtable_delete(ht, NULL);
  bag_delete(bag,webpage_delete);
}


//takes in the current page, and a bag and hashtable
//scans through the whole page to find URLs present in the page
//populates the bag with new pages based on those URLs
//populates the hashtables with the URLs
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen) {
  
  int pos = 0;
  int depth = webpage_getDepth(page)+1; //increment depth one greater than the current wepbages depth
  char* url;
  
  while ((url = webpage_getNextURL(page,&pos)) != NULL) { //keep on looping until no more URLs
    char* normalizedURL = normalizeURL(url); //normalize the URL
    if (isInternalURL(normalizedURL)) { //check if it is an internal URL
      
      char* copy = malloc(sizeof(char) * strlen(normalizedURL) + 1); //copy over the URL and malloc the copy
      if (copy == NULL) {//error check the malloc
        exit(6);
      }
      strcpy(copy, normalizedURL); //copy the normalized URL to the copy variable
      if (hashtable_insert(pagesSeen,copy,"z")) { //attempt to insert the url into the hashtable
        webpage_t* newPage = webpage_new(copy,depth,NULL); //create a new page to be inserted into bag later
        if (newPage == NULL) { //error check newPage creation
          fprintf(stderr,"Memory Allocation error when allocating New page \n");
          exit(8);
        }
				bag_insert(pagesToCrawl,newPage); //insert newPage into the bag
        
			}
      else {
        free(copy); //in case hashtable_insert returns false
      }
      
    }
    //free both the url's since they are no longer needed
    free(normalizedURL); 
    free(url);
  }
}

