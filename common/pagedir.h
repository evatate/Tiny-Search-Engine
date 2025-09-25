/* 
 * pagedir.h
 *
 * Eva Tate CS50, Dartmouth
 */

#ifndef PAGEDIR_H
#define PAGEDIR_H

#include <stdio.h>
#include <stdbool.h>
#include "webpage.h"


/**************** pagedir_init ****************/
/* Save a webpage into the pageDirectory
 *
 * Caller provides:
 *  a directory
 * We return:
 *  a boolean if the program worked
*/
bool pagedir_init(const char* pageDirectory);

/**************** pagedir_save ****************/
/* Save a webpage into the pageDirectory based on docID
 *
 * Caller provides:
 *  a webpage, a directory, and a docId
 * We return:
 *  nothing
*/
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);

/**************** pagedir_load ****************/
/* load a webpage based on the contents of the specified crawler output file
 *
 * Caller provides:
 *  a string page directory, and an integer for document ID
 * We return:
 *  the webpage with all the information like url, depth, html
*/
webpage_t* pagedir_load(char* pageDirectory, int docID);

/**************** pagedir_validate ****************/
/* load a webpage based on the contents of the specified crawler output file
 *
 * Caller provides:
 *  a string page directory, and an integer for document ID
 * We return:
 *  the webpage with all the information like url, depth, html
*/
bool pagedir_validate(char* pageDirectory);


#endif // __PAGEDIR_H
