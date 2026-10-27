# Operating-Systems-Project-1


# Linear Hashing & Dynamic Elector Management System

A C++ implementation of **Linear Hashing** and **Dynamic Hashing** concepts, featuring a dynamically expanding hash table, a reverse index, and an interactive command-line interface (terminal like) for managing elector records.

## Features

- **Linear Hashing:** Incremental bucket splitting, dynamic table growth, and overflow bucket management.
- **Elector Management:** Insert and search records using unique PINs, with duplicate detection.
- **Reverse Index:** Organize electors who have voted by postal code.
- **Voting Statistics:** Track voter counts, participation ratios, and postal-code groups.
- **Interactive CLI:** Command-driven interface for managing records and inspecting internal data structures.
- **File-Based Initialization:** Load elector records from a text file.
- **Memory Management:** Manual dynamic allocation and deallocation of data structures.

## What is Linear Hashing?

**Linear Hashing** is a dynamic hashing technique that allows a hash table to grow incrementally without reorganizing the entire table. Instead of doubling the table all at once, it splits one bucket at a time, using a split pointer to track the next bucket to expand.

### Hash Function

The implementation uses a family of hash functions:

\[
h_i(k) = k \bmod (2^i \cdot m)
\]

Where:
- \(k\) is the elector's PIN (key).
- \(m\) is the initial number of buckets.
- \(i\) is the current hashing round.

Initially, \(i=0\), so the hash function is:

\[
h_0(k) = k \bmod m
\]

As the table grows, the next-level hash function uses twice the modulus, allowing records to be distributed across a larger number of buckets.

### Bucket Splitting

The split pointer identifies the next bucket to split. When the load factor exceeds the configured threshold of **0.75**, a new bucket is allocated and the records in the selected bucket are redistributed using the next-level hash function. Buckets are split sequentially, and when a round is complete, the hashing level increases and the pointer returns to the first bucket.

The load factor is calculated as:

\[
\alpha = \frac{N}{B \cdot C}
\]

Where:
- \(N\) is the number of stored records.
- \(B\) is the number of primary buckets.
- \(C\) is the capacity of each bucket.

### Collision Handling

When a bucket reaches its capacity, the implementation uses **overflow buckets linked together in a chain** to accommodate additional records. During a split, records from the primary bucket and its overflow chain are redistributed according to the updated hash function.

This combination of incremental splitting and overflow handling allows the table to adapt to growing datasets while maintaining efficient search and insertion operations.



## Build & Run

**Requirements:** `g++`, `make`, and a Unix-like terminal.

Compile the project:

```bash
make all
```

Run with an input file, bucket capacity, and initial bucket count like: 

```bash
./final -f <file> -b <integer> -m <integer>
```

e.g. : 

```bash
./final -f voters500.csv -b 2 -m 4
```

Clean build artifacts:

```bash
make clean
```

## Interactive Commands

The interactive command line interpreter acts like a terminal that includes prompts to manage and handle the whole application. You can see the implementation in the files Prompt.cpp and Prompt.h. The executable displays its own prompt in the terminal like so:

```bash 
--->
```

And then the user can type the following commands/prompts.  The program reads the input, checks the command and its arguments, performs the requested operation, displays the result, and waits for the next command. If the command doesn't exist it diplays it in the terminal and waits for the next input.


| Command | Description |
|---|---|
| `i <pin> <lname> <fname> <zip>` | Insert an elector |
| `l <pin>` | Search for an elector |
| `m <pin>` | Mark an elector as having voted |
| `bv <file>` | Mark electors as voted using a file of PINs |
| `v` | Display the number of voters |
| `perc` | Calculate the voting participation ratio |
| `z <zipcode>` | List voters by postal code |
| `o` | Sort postal-code groups by voter count |
| `pht` | Display the internal hash-table structure |
| `pri` | Display the reverse index |
| `t` | Display the total number of electors |
| `exit` | Terminate the application |

The implementation also handles leading and trailing spaces or tabs like a real bash terminal. For example:

```bash
--->      l    1001 
```

is intended to be treated as the command: l 1001.

## Input Format

The input file must contain one elector per line, with four whitespace-separated fields:

```text
PIN LASTNAME FIRSTNAME POSTALCODE
```

Example:

```text
1001 PAPADOPOULOS NIKOS 11111
1002 GEORGIOU MARIA 22222
1003 DIMITRIOU ELENI 11111
```

## Key Learning Outcomes

- Implementing dynamic hashing and incremental bucket splitting.
- Managing collisions through overflow chains.
- Designing and maintaining a secondary index using linked lists.
- Applying modular programming, file processing, and interactive command parsing in C++.
- Managing dynamically allocated memory and analyzing data-structure performance.
