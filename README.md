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

## Technologies

- C++
- Data Structures & Algorithms
- Linear Hashing
- Linked Lists
- Dynamic Memory Management
- Makefile / GNU Make

## Build & Run

**Requirements:** `g++`, `make`, and a Unix-like terminal.

Compile the project:

```bash
make all
```

Run with an input file, bucket capacity, and initial bucket count:

```bash
./final -f electors.txt -b 2 -m 4
```

Clean build artifacts:

```bash
make clean
```

## Interactive Commands

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
