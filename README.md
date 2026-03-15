# Spell Checker

---

## Table of Contents

- [Purpose of the application](#purpose-of-the-application)
- [Quickstart](#quickstart)
  - [Setup](#setup)
  - [Usage](#usage)
- [Technical Description](#technical-description)
  - [Architecture](#architecture)
  - [dictionary.h](#dictionaryh)
  - [dictionary.c](#dictionaryc)
    - [Data structures](#data-structures)
    - [hash(word)](#hashword)
    - [load(dictionary)](#loaddictionary)
    - [check(word)](#checkword)
    - [size()](#size)
    - [unload()](#unload)
  - [speller.c](#spellerc)
- [Sample Output](#sample-output)
- [Testing](#testing)
- [Constraints](#constraints)
- [License](#license)

---

## Purpose of the application

This application is a hash table–based spell checker written in C, built as part of [CS50x](https://cs50.harvard.edu/x/). It loads a 143,091-word dictionary into memory, checks every word in an input text file against the dictionary, and prints all misspelled words. At the end of each run, it reports benchmarks for the time spent in each stage: loading the dictionary, checking words, computing the dictionary size, and unloading.

---

## Quickstart

### Setup

This application requires a number of files provided by CS50, which includes `speller.c`, `Makefile`, the `dictionaries/` folder, and sample `texts/` folder. Download the files from [cs50](https://cdn.cs50.net/2023/fall/psets/5/speller.zip)

```bash
unzip speller.zip
cd speller
```

Then replace the placeholder `dictionary.c` and `dictionary.h` with the files from this repository. Compile with:

```bash
make speller
```

> **Note:** The `Makefile`'s default target is `speller`, which compiles and links both `speller.c` and `dictionary.c`. Running `make dictionary` alone will not produce a working file.

### Usage

```bash
./speller [dictionary] text
```

Omitting `[dictionary]` defaults to `dictionaries/large`. Examples:

```bash
./speller texts/lalaland.txt
./speller dictionaries/small texts/cat.txt
```

---

## Technical Description

### Architecture

The application is organised across the following files:

| File | Source | Role |
|---|---|---|
| `dictionary.h` | This repo | Declares all function prototypes and defines `LENGTH 45` |
| `dictionary.c` | This repo | Hash table implementation: `load`, `check`, `hash`, `size`, `unload` |
| `speller.c` | CS50 | Driver: reads text, calls dictionary functions, times each stage |
| `Makefile` | CS50 | Links `speller.o` and `dictionary.o` into the `speller` binary |
| `dictionaries/large` | CS50 | 143,091 lowercase words — the default dictionary |
| `dictionaries/small` | CS50 | Tiny dictionary useful for debugging |
| `texts/` | CS50 | Sample texts to spell-check (Shakespeare, Tolstoy, La La Land, etc.) |

`speller.c` acts as the driver and is provided by CS50. It handles file I/O, word extraction, and timing. The developer-implemented logic lives entirely in `dictionary.c`, which is compiled against the interface declared in `dictionary.h`.

---

### dictionary.h

Declares the five function prototypes used by both `dictionary.c` and `speller.c`, and defines the constant `LENGTH 45` — the maximum number of characters a valid word may contain.

---

### dictionary.c

Implements the dictionary as a hash table with **81 buckets** (`N = 9 × 9`) using **separate chaining** — each bucket holds a singly linked list of `node` structs to handle hash collisions. Each `node` stores a word string and a pointer to the next node in the chain.

Two global variables track state: `dictionary_size` (an unsigned long incremented on each `load` insertion) and `dictionary_loaded` (a boolean set to `true` once loading is complete).

#### Data structures

The hash table is declared as `node *table[N]`, where `N = 81`. Each element is a pointer to the head of a linked list of `node` structs. A `node` has two fields:

- `word` — a character array of length `LENGTH + 1` storing the word
- `next` — a pointer to the next node in the chain

#### hash(word)

The hash function maps a word to a bucket index between 0 and 80 using two properties of the word:

1. **Word length** — `strlen(word) % 9` → a `length_index` from 0–8
2. **ASCII sum** — the sum of all lowercased character values `% 9` → an `ascii_index` from 0–8

The final bucket index is `length_index * 9 + ascii_index`. This two-dimensional approach distributes words across buckets based on both length and character composition, reducing collisions compared to a single-property hash.

#### load(dictionary)

This function opens the dictionary file and reads it word by word into the hash table. For each word, it:

1. Allocates memory for a new `node`.
2. Copies the word into the node's `word` field.
3. Calls `hash` to determine the bucket index.
4. Inserts the node at the **head** of the corresponding linked list.
5. Increments `dictionary_size`.

If the dictionary file cannot be opened, or if any memory allocation fails, the function frees any already-allocated memory, closes the file, and returns `false`. On success it sets `dictionary_loaded` to `true` and returns `true`.

#### check(word)

This function takes a word as input, hashes it to find the correct bucket, and traverses the linked list at that bucket using `strcasecmp` for case-insensitive comparison. It returns `true` if a match is found and `false` otherwise.

#### size()

Returns `dictionary_size` if `dictionary_loaded` is `true`, and `0` otherwise. It is called by `speller.c` after loading is complete to report the number of words in the dictionary.

#### unload()

Frees all memory used by the hash table. It iterates over every bucket in `table[N]` and, for each non-empty bucket, traverses the linked list and frees each node one by one until the list is empty. Returns `true` on completion.

---

### speller.c

Provided by CS50 and not modified. It acts as the driver for the application:

1. Parses command-line arguments to determine the dictionary and text file to use.
2. Calls `load` to load the dictionary into memory and benchmarks the time taken.
3. Opens the text file and reads it character by character, extracting valid words (alphabetical characters and apostrophes, up to `LENGTH` characters long).
4. Calls `check` on each word and prints it if misspelled, benchmarking the cumulative check time.
5. Calls `size` to retrieve the number of words in the dictionary.
6. Calls `unload` to free the dictionary from memory and benchmarks the time taken.
7. Prints a summary report of misspelled words, dictionary size, word count, and timing benchmarks.

The `calculate` helper function computes the elapsed time between two `getrusage` snapshots in seconds.

---

## Sample Output

```
MISSPELLED WORDS

WORDS MISSPELLED:     955
WORDS IN DICTIONARY:  143091
WORDS IN TEXT:        17756
TIME IN load:         0.03
TIME IN check:        0.01
TIME IN size:         0.00
TIME IN unload:       0.00
TIME IN TOTAL:        0.04
```

---

## Testing

**Memory leaks** — verify with `valgrind`:

```bash
valgrind ./speller texts/cat.txt
```

---

## License

Completed as part of CS50x.
