/* 
 * pagedir.c
 *
 * see pagedir.h for more information
 *
 * Eva Tate CS50, Dartmouth
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "webpage.h"
#include "mem.h"
#include "file.h"

bool pagedir_init(const char* pageDirectory);
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);
webpage_t* pagedir_load (char* pageDirectory, int docID);
bool pagedir_validate(char* pageDirectory);

//intialize the main directory where crawler outputs will go
bool pagedir_init(const char* pageDirectory) {
    
    //allocate memory for the pathname
    char* pathname = mem_malloc(strlen(pageDirectory)+10);
    if (pathname == NULL) { //check memory allocation
        fprintf(stderr,"Allocation error \n");
        free(pathname);
        return false;
    }

    //copy over the pathname from page directory, and appaned the .crawler file
    strcpy(pathname,pageDirectory);
    strcat(pathname,"/.crawler");

    //open the file and make it writable
    FILE* fp = fopen(pathname,"w");
    if (fp == NULL) {
        fprintf(stderr,"This file could not be opened\n");
        return false;
    }
    fclose(fp);
    free(pathname);


    return true;
}

//function to take in a page, document ID, and page directory
//output is the url of the page, it's depth, and the HTML outputs
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID) {
    //allocate memory for pathname and docID
    char* pathname = mem_malloc(strlen(pageDirectory)+sizeof(docID)+3);
    char* docIDString = mem_malloc(sizeof(char)*12);
    if (pathname == NULL || docIDString == NULL) {
        fprintf(stderr,"Allocation error \n");
    }

    //convert the docID into a string
    sprintf(docIDString,"%d",docID);
    strcpy(pathname,pageDirectory);
    strcat(pathname,"/");
    //the pathname of the new document is the directory, 
    //and the name is the docID in string form
    strcat(pathname,docIDString);

    //opens the file and makes it writable
    FILE* fp = fopen(pathname,"w");
    if (fp == NULL) {
        fprintf(stderr,"This file could not be opened\n");
        return;
    }
    //print out utrl html and depth to the document
    fprintf(fp, "%s\n", webpage_getURL(page));
    fprintf(fp, "%d\n", webpage_getDepth(page));
    fprintf(fp, "%s", webpage_getHTML(page));
    fclose(fp);
    //remember to free the intial two mallocs
    free(pathname);
    free(docIDString);
    return;

}

//function to read through a crawler output file
//loads in a webpage based on that crawler output file with a url, depth, html
webpage_t* pagedir_load(char* pageDirectory, int docID) {
  char* fileHTML;
	char* url;
	int depth = 0;
  char* depthString;


	char* filename = malloc(sizeof(char)*strlen(pageDirectory)+sizeof(char)*6+1);
  
	if (filename == NULL) {
		fprintf(stderr, "memory allocation error\n");
		exit(2);
	}

  //combines pageDirectory and the document ID into one string called filename
	sprintf(filename, "%s/%d", pageDirectory,docID);

  //opens the file
  FILE* fp;
  fp = fopen(filename,"r");
  if (fp==NULL) {
    free(filename);
    return NULL;
  }
  webpage_t* page;

  //reads the first line of the document, the URL
	url = file_readLine(fp);
  //reads the second line of the document, the Depth
	depthString = file_readLine(fp);
  //converts into int
  sprintf(depthString, "%d", depth);
  //reads the rest of the file, the HTML
  fileHTML = file_readFile(fp);
  if (fileHTML == NULL) {
    fprintf(stderr,"Error loading file into HTML\n");
    exit(5);
  }
  //creates a new webpage with this information
  page = webpage_new(url,depth,fileHTML);
	fclose(fp);
	free(filename);
  free(depthString);
	
	return page;
}

//function to validate wether a directory is the output of a crawler sequence
bool pagedir_validate(char* pageDirectory) {
	char* filename = malloc(sizeof(char)*strlen(pageDirectory)+sizeof(char)*10+1);
	//adds "/.crawler to the end of page directory"
  strcpy(filename,pageDirectory);
	strcat(filename,"/.crawler");
  //attempt to open the crawler file if it is present
	FILE* fp = fopen(filename,"r");
	free(filename);
	if (fp==NULL) {
		fprintf(stderr,"This directory is not a crawler directory\n");
		return false;
	}
	else {
		fclose(fp);
		return true;
	}
}



