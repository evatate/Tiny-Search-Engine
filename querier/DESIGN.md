# CS50 TSE Querier
## Design Spec

Inspired by Design Spec of Crawler.

According to the [Querier Requirements Spec](REQUIREMENTS.md), the TSE *querier* is a standalone program that reads the index file produced by the TSE Querier, and page files produced by the TSE Querier, and answers search queries submitted via stdin.


### User interface

The queriers's only interface with the user is on the command-line; it must always have two additional arguments.

```
querier pageDirectory indexFilename
```

For example, if `data1` is a pageDirectory in `../indexer`,

``` bash
$ ./querier ../indexer/data1 ../indexer/output1.txt
```

### Inputs and outputs

**Input**: The input is a query that the user determines, and can hold words like "and" or "or" and can have as many spaces in between words. The input needs to be only words, not numbers or other special characters. 

**Output**: We output the top results, in the format of a URL and the respective score according to the query.

### Functional decomposition into modules

We anticipate the following modules or functions:

 1. *main*, which holds all the arguments and begins the parsing of the query
 2. *parseArgs*, validates the directory and the filename, makes sure users command is correct.
 3. *processQuery*, does all the work of processing the query, starting from reading an input from stdin. Parses each input line into an array of words, validates the query, then calculates the score. 
 4. *parseInputIntoArrayAndCountWords*, parses a input line of type char* into an array of char* and cuts off spaces, returns count of words in the array.
 5. *validateQuery*, checks wether the query is a valid query of words, and if two "and/or" characters are at the start, end, or adjacent to each other
 6. *calculateScore*, calculates score based on the amount of times a word shows up in a document. Has to consider the expression type and how to score in the prescence of implict ands, or literal "and/or".
 7. *sortScores*, creates an array of pairs of docID's and scores, and sorts them with a selection sort and prints out the top scores.

And some helper functions that provide data structures:

 1. *filesInDirectory*, calculates the number of files in crawler directory
 2. *isAndOr*, checks if a word is an "and" or an "or" character
 3. *countersIntersection*, finds the intersection of two counters, essentially finds all the documents where both the words exist
 4. *countersUnion*, finds the union of two counters, essentially finds all the documents where either one of the words exists

And some helper modules that provide data structures:

 1. *index*, a module providing the data structure to represent the in-memory index, and functions to read and write index files;
 1. *webpage*, a module providing the data structure to represent webpages, and to scan a webpage for words;
 2. *pagedir*, a module providing functions to load webpages from files in the pageDirectory;
 4. *word*, a module providing a function to normalize a word.

### Pseudo code for logic/algorithmic flow

The querier will run as follows:

  	parse the command line, validate parameters, initialize other modules
    call function to parse query into words, and process that query (look at implementation for pseudocode), then prints out the highest scoring documents and their URLs

where *parseInputIntoArrayAndCountWords:*

		loop through each character of the input
			if any of these characters are not spaces or not alphabetic characters, throw error

		loop through each character starting from beggining again
			intialize start index at current position
			loop until a space is found
			initialize end index at that position

			if wordLength is greater than 1
				create a string that equals the word from startIndex to endIndex in inputLine
				Add that word to an array of strings
				increment word count
		
		return word count

where *validateQuery:*

	check if the first word is an and/or
	check if the last word is an and/or

	loop through every word in the array
		check if the previous word and the current word are and/or
		if there are adjacent and/or's
			return that the query is not valid - false

	if there are no adjacent and/or's
		return that the query is valid - true

*calculateScore:*

	create a new results counter
	create a null temp counter

	loop through every word in the words array
		if the word is not an and/or
			if the temp counter is null
				set temp to the words counter in index
			else
				find the intersection of temp and the words counter
		if the word is an or
			find the union of results and temp
			set temp back to null
		if the word is an and
			continue onwards and treat it is an implicit and
	
	find the union between the result and temp counters
	return the result counter to be ranked

*sortScores:*

	create an array of docScorePairs which starts empty
	loop through every file in the crawler directory
		find the score of the current documentID
		if the score is greater then 0
			initialize a docScorePair with the respective docID and score
			add that docScorePair to the array
			increment a counter of pairs added
	
	if the amount of scores in the array are less than 0
		alert user that no results were found
	if the amount of scores in the array were more than 0
		use qsort to sort the array in descending order
		loop through the sorted array
			load a webpage with the correct information based on the document
			print out each document with it's score, docID, and URL
			delete the webpage

	make sure to delete the count
	make sure to free the pairs array


### Counters Union and Intersection Logic

*countersIntersection:*

	The intersection of two counters is essentially strictly where they overlap.
	Let's say you have two sets, A and B, the intersection of these two sets would be the values in A and B, that are present in BOTH A and B. 
	Intersections are notated as A ^ B

*countersUnion:*

	The union of two counters is essentially the values that exist in either counter. 
	Let's say you have two sets, A and B, the union of these two sets would be the values in A or in B, that are present in EITHER A and B. 
	Unions are notated as A u B.

*Example:*

	Intersection: [1,2,3.4] ^ [1,3,5,8] = [1,3]
	Union: [1,2,3.4] u [1,3,5,8] = [1,2,3,4,5,8]
	


### Major data structures

The key data structure is the *index*, mapping from *word* to *(docID, #occurrences)* pairs.
The *index* is a *hashtable* keyed by *word* and storing *counters* as items.
The *counters* also another key data structure, which is keyed by *docID* and stores a count of the number of occurrences of that word in the document with that ID.

### Custom Structs

*docScorePair:*

	Used to map a document ID to a score based on a query
	Holds an integer document ID and an integer score


*two_counters:*

	A struct used when iterating through counters to find unions and intersections
	Essentially holds two counters in one struct, a result and temporary counter

### Testing plan

*Integration testing*.  The *querier*, as a complete program, will be tested by typing out a valid query, and the ensuing output will be the documents/urls with the highest count of the words present in the query.

1. Test `querier` with various invalid arguments.
	2. no arguments
	3. one argument
	4. three or more arguments
	5. invalid `pageDirectory` (non-existent path)
	6. invalid `indexFile` (non-existent path)
0. Run *querier* on a variety of different inputs that mix and's and or's, and check wether the order of expressions is correctly done, do this through fuzz testing and `fuzzquery.c`
0. Run *valgrind* on *querier* to ensure no memory leaks present in valgrind.