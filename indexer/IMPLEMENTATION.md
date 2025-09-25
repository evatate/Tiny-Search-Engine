# CS50 TSE Indexer
## Implementation Spec

In this document we reference the [Requirements Specification](REQUIREMENTS.md) and [Design Specification](DESIGN.md) and focus on the implementation-specific decisions.
The knowledge unit noted that an [implementation spec](https://github.com/cs50dartmouthwi24/home/blob/main/knowledge/units/design.md#implementation-spec) may include many topics; not all are relevant to the TSE or the Indexer.
Here we focus on the core subset:

-  Data structures
-  Control flow: pseudo code for overall flow, and for each of the functions
-  Detailed function prototypes and their parameters
-  Error handling and recovery
-  Testing plan

## Data structures 

We use two data structures: a 'bag' of pages that need to be crawled, and a 'hashtable' of URLs that we have seen during our crawl.
Both start empty.
The size of the hashtable (slots) is impossible to determine in advance, so we use 200.

## Control flow

The Indexer is implemented in one file `indexer.c`, with three functions.

### main

The `main` function simply calls `indexBuild and then `index_delete`, then exits zero. The main function also parses all the arguments to make sure directories are valid, and files are valid. 

### indexBuild

Do the real work of indexing from `pageDirectory`/`docID` and saving the result of the index to the specified output file. 

Pseudocode:

  creates a new 'index' object
  loops over document ID numbers, counting from 1
    loads a webpage from the document file 'pageDirectory/id'
    if successful, 
      passes the webpage and docID to indexPage
### indexPage

This function loads an individual `webpage`, parses through each word in that page, and uses `index_addWord` to add each word to the index. Utilizes the `webpage_getNextWord` function to loop through every valid word in the document that is greater than 2 characters.

Pseudocode:

	 steps through each word of the webpage,
   skips trivial words (less than length 3),
   normalizes the word (converts to lower case),
   looks up the word in the index,
     adding the word to the index if needed
   increments the count of occurrences of this word in this docID

## Other modules

### pagedir

We create a re-usable module `pagedir.c` to handle validating wether a directory is a valid `crawler` directory with a `/.crawler` file. Also handles creating a new webpage based on a `/.crawler file`.
We chose to write this as a separate module, in `../common`, to encapsulate all the knowledge about how to initialize and validate a pageDirectory, and how to write and read page files, in one place... anticipating future use by the Querier.

Pseudocode for `pagedir_load`:

	read through line 1 of the file to get the url
	read through line 2 of the file to get the depth
	read through the rest of the file to get the html
	create a webpage with this information


Pseudocode for `pagedir_validate`:

	create a test pathname and filename that appends /.crawler to path directory
	open up that file to make sure it exists
	if it does, validate the directory

### libcs50

We leverage the modules of libcs50, most notably `hashtable`, `counter` and `webpage`.
See that directory for module interfaces.
The new `webpage` module allows us to represent pages as `webpage_t` objects, and most importantly parse through a document and use `webpage_getNextWord` to read in every word from the HTML. 

### indextest
`indextest` essentially creates a new index using the `index_load` function from the `index` module, and reverse engineers the index instead of getting 
input from a crawler output file. Then proceeds to save the index to an external file.

### index (Module)

The index module is essentially an aliased hashtable, with some similar functionality and some additional methods. The index will always be a hashtable with key word, and a value of counter, where the key of the counter is the docID and the value of the counter is the number of times that word shows up in that document. We can create new indexes, add to indexes, delete indexes, iterate through them, check if words exist in an index, and print an index to a file. The index module also hold support for the core of `indextest`
with the `index_load` function.

```c
typedef hashtable_t index_t;
index_t* index_new(const int numSlots);
bool index_insert(index_t* index, char* keyWord, counters_t* counter);
void index_iterate(index_t* index, void* arg, 
			void(*itemfunc)(void* arg, const char* key, void* item));
void index_delete(index_t* index);
counters_t* index_find(index_t* index, const char* keyWord);
void index_print(index_t* index, char* filename);
void index_addWord(index_t* index, char* word, int docID);
index_t* index_load(char* oldFile);
```


## Function prototypes

### indexer

Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's implementation in `crawler.c` and is not repeated here.

```c
void indexPage(index_t* index, webpage_t* page, int docID);
void indexBuild(char* pageDirectory,char* outputFilename);
```

### pagedir

Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's declaration in `pagedir.h` and is not repeated here.

```c
webpage_t* pagedir_load (char* pageDirectory, int docID);
bool pagedir_validate(char* pageDirectory);
```

## Error handling and recovery

All the command-line parameters are rigorously checked before any data structures are allocated or work begins; problems result in a message printed to stderr and a non-zero exit status.

## Testing plan

Here is an implementation-specific testing plan.

### Unit testing

The indexer represents the whole system and utilizes the index module.
The pagedir unit is tiny; it could be tested using a small C 'driver' to invoke its functions with various arguments, but it is likely sufficient to observe its behavior during the system test.

### Regression testing

The indexer can take a long time to run on some crawler output files
For routine regression tests, we index the result of the `letters` crawler output at moderate depths;

### Integration/system testing

We write a script `testing.sh` that invokes the indexer several times, with a variety of command-line arguments.
First, a sequence of invocations with erroneous arguments, testing each of the possible mistakes that can be made.
Third, runs through a set of crawler output files, and created index files.
Verify correct behavior by studying the output, and by sampling the files created in the respective pageDirectories. Also tests indextest on one of those output files.