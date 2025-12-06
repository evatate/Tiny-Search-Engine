# Tiny Search Engine (TSE)

A full-featured search engine implemented from scratch in C, including a **web crawler**, **indexer**, and **query processor**. Designed as a miniature Google, this project involves efficient data structures, algorithmic design, and systems-level programming for high-performance information retrieval.

---

## Overview

The Tiny Search Engine implements a complete search pipeline:

1. **Web Crawler** – Traverses seed URLs to fetch web pages, extract and clean text content.
2. **Indexer** – Constructs an inverted index mapping terms to documents with frequency and positional data.
3. **Query Processor** – Accepts user queries, searches the index efficiently, and returns ranked results based on relevance.

The project emphasizes modularity, memory efficiency, and scalability, with careful management of low-level details in C.

---

## Architecture Overview

```
+----------------+       +----------------+       +----------------+       +----------------+
|   Web Crawler  | ----> |    Indexer     | ----> | Inverted Index | <----> | Query Processor |
|  (Fetch Pages) |       | (Parse/Store)  |       | (Term → DocID) |       | (Search & Rank) |
+----------------+       +----------------+       +----------------+       +----------------+
        │                        │                        │                        │
        │ Fetch URLs & HTML      │                        │                        │
        │---------------------->│                        │                        │
        │                        │ Tokenize & normalize    │                        │
        │                        │ words                  │                        │
        │                        │---------------------->│ Build/update index     │
        │                        │                        │                        │
        │                        │                        │ Queries access term → DocIDs
        │                        │                        │<----------------------│
        │                        │                        │                        │
        │                        │ Store index to disk     │                        │
        │                        │<----------------------│                        │
        │                        │                        │                        │
        │                        │                        │ Return ranked results │
        │                        │                        │---------------------->│
```

---

## Query-Time Retrieval Flow

```
User Query
     │
     ▼
+----------------+
| Query Processor|
+----------------+
     │
     │ Tokenize & normalize query terms
     ▼
+----------------+
| Inverted Index |
| (Term → DocID) |
+----------------+
     │
     │ Lookup each term → retrieve posting lists
     ▼
+----------------+
| Merge Posting  |
| Lists          |
+----------------+
     │
     │ Compute relevance score per document (TF-IDF)
     ▼
+----------------+
| Ranking Engine |
+----------------+
     │
     │ Sort documents by relevance
     ▼
+----------------+
|  Result List   |
+----------------+
     │
     ▼
   Return top-k documents
```

### Flow Explanation

1. **Query Processor:** Normalizes input, removes stop words, and optionally stems terms.
2. **Inverted Index Lookup:** Retrieves posting lists containing document IDs and term frequencies.
3. **Merge Posting Lists:** Efficiently combines lists for multi-term queries.
4. **Relevance Scoring:** Computes TF-IDF or similar scores to rank results.
5. **Ranking Engine:** Sorts candidate documents and returns the top-k most relevant.
6. **Result List:** Displays ranked documents to the user.

---

## Features

* Full search engine implemented in **C** from scratch.
* Modular components: crawler, indexer, query processor.
* Efficient, memory-conscious data structures: hash tables, tries, and linked lists.
* Supports term frequency and positional data for relevance scoring.
* Incremental index updates without full rebuilds.
* Query-time relevance ranking using TF-IDF.
* Designed for performance, scalability, and extensibility.

---

## Tech Stack

* **Data Structures:** Hash tables, tries, linked lists for fast indexing and retrieval.
* **Memory Management:** Manual allocation/deallocation for large datasets.
* **Algorithms:** Tokenization, normalization, stop-word removal, inverted indexing, TF-IDF ranking.
* **Modularity:** Components operate independently for scalability and testing.
* **Performance Optimizations:**

  * Incremental indexing.
  * Efficient posting list merging for multi-term queries.
  * Minimal memory overhead and fast retrieval.

---

## Usage

1. **Compile the project:**

```bash
make
```

2. **Run the web crawler:**

```bash
./crawler seed_urls.txt output_folder
```

3. **Build the index:**

```bash
./indexer output_folder index_file
```

4. **Run the query processor:**

```bash
./query_processor index_file
```

5. Enter search terms to retrieve top-ranked document results.

# Testing
Running testing.sh will create three output files, output.txt, output1.txt, indexOutput.txt


