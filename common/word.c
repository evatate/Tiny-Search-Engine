/* 
 * word.c
 * This file takes in a word and turns it into all lowercase
 * see word.h for more information
 * Eva Tate CS50, Dartmouth
 */

#include<string.h>
#include<ctype.h>

//function to normalize a given word
//essentially turns the full word into lowercase letters
char* normalizeWord(char* word) {
  if (word==NULL) {
    return word;
  }
  else {
    //loops through every characters and makes it lowercase
    for (int i = 0; i<strlen(word); i++) {
      word[i] = tolower(word[i]);
    }
  }
  return word;

}
