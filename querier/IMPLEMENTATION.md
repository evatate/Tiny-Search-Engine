# CS50 TSE Querier
## Implementation Spec

In this document we reference the [Requirements Specification](REQUIREMENTS.md) and [Design Specification](DESIGN.md) and focus on the implementation-specific decisions. Inspired by Implementation Spec of Crawler.
The knowledge unit noted that an [implementation spec](https://github.com/cs50dartmouthwi24/home/blob/main/knowledge/units/design.md#implementation-spec) may include many topics; not all are relevant to the TSE or the Querier.
Here we focus on the core subset:

-  Data structures
-  Control flow: pseudo code for overall flow, and for each of the functions
-  Detailed function prototypes and their parameters
-  Error handling and recovery
-  Testing plan

## Data structures 

The key data structure is the *index*, mapping from *word* to *(docID, #occurrences)* pairs.
The *index* is a *hashtable* keyed by *word* and storing *counters* as items.
The *counters* also another key data structure, which is keyed by *docID* and stores a count of the number of occurrences of that word in the document with that ID.

## Control flow

The Querier is implemented in one file `querier.c`, with multiple core functions, and additional helper functions. 

### main

The `main` function simply calls `parseArgs` and begins the processing of the query, then exits zero.

### parseArgs

Given arguments from the command line, extract them into the function parameters; return only if successful.

* for `pageDirectory`, call `pagedir_validate()` to make sure it is a valud cralwer directory
* for `indexFile`, check wether the file is able to be opened and read
* if any trouble is found, print an error to stderr and exit non-zero.

### processQuery

Do the real work of reading in a query, processing the query, calculating the scores for each document, and printing out the sorted scores.
Check DESIGN.md for pseduocde for helper functions used in *processQuery*.

Pseudocode:

  starts a while loop, only breaks when specifed by user
    read in line of input from stdin
    create an array of strings
    call *parseInputIntoArrayAndCountWords:*
    if number of words is greater than 0
      print out the cleaned query
      if the query is a valid query
        call *calculateScore:* and store it in a counter
        call *sortScores:* on that counter
        free everything related to query

## Other modules

### libcs50

We leverage the modules of libcs50, most notably `counter`, `hashtable`, and `webpage`.
See that directory for module interfaces.

### indexer
We also leverage the output index files that indexer creates.
See that directory for module interfaces.

### crawler
We also leverage the directory of files that crawler creates.
See that directory for module interfaces.

## Function prototypes

### querier

Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's implementation in `querier.c` and is not repeated here.

```c
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
```

## Error handling and recovery

All the command-line parameters are rigorously checked before any data structures are allocated or work begins; problems result in a message printed to stderr and a non-zero exit status.

All code uses defensive-programming tactics to catch and exit (using variants of the `mem_assert` functions), e.g., if a function receives bad parameters.

That said, certain errors are caught and handled internally: for example, `pagedir_init` returns false if there is any trouble creating the  `.crawler` file, or `pagedir_validate` returns false if it is not a crawler directory, allowing Querier to decide what to do from there on. 

## Testing plan

Here is an implementation-specific testing plan.

### Unit testing

There is only one unit (querier).
The querier represents the whole system and is covered below.

### Regression testing

The querier can take a long time to run on some sites when `maxDepth` in the crawler directory is more than 2.
For routine regression tests, we crawl the `letters` site at moderate depths

### Integration/system testing

We write a script `testing.sh` that invokes the querier several times, with a variety of command-line arguments.
First, a sequence of invocations with erroneous arguments, testing each of the possible mistakes that can be made.
Second, a run with valgrind over a moderate-sized test case (such as `toscrape` at depth 1).
Run that script with `bash -v testing.sh` so the output of querier is intermixed with the commands used to invoke the querier.
Verify correct behavior by studying the output, and by sampling the files created in the respective pageDirectories.

### Fuzz Testing

We use a method testing called fuzz testing in the testing of querier, which essentially generates a specified number of random inputs, and pipes them into the querier so we can test random inputs to see if the querier has any logical fallacies, or can't handle boundary cases.

> For Lab 4, as noted in the assignment, you may submit a smaller test run.
> Furthermore, we recommend turning off detailed logging output for these tests, as they make `testing.out` rather large!