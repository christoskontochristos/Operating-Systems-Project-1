// linear_hash_table.h

#ifndef LINEAR_HASH_TABLE_H
#define LINEAR_HASH_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Elector.h"
#include "reverse_index.h"

//constants
const float LOAD_FACTOR_THRESHOLD = 0.75; //the load factor threshold

static size_t bytes; //global variable to count the number of bytes that have been freed


//struct for Bucket
struct Bucket {
    Elector **Array; //the array of the bucket
    int index_number; //the bucket's index number 
    struct Bucket *next; //pointer to the next bucket (overflow buckets)
};

//Struct for LinearHashTable
struct LinearHashTable{
    Bucket **array; //array for the table 
    Bucket *pointer; //pointer to point at the bucket to be split
    int m; //the initial number of buckets
    int bucket_size; //the bucket size
    int capacity; //the total number of electors in the hash table 
    int round; //the round
    int round_count; //utility variable to help check for the new round
    int current_size; //the number of non-overflow buckets
};

//Function prototypes
struct LinearHashTable *InitHashTable(int bucket_size,int bucket_num);
void InsertKey1(LinearHashTable *table, Elector *elector);
void InsertKey(LinearHashTable *table, Elector *elector);
Elector *Search(LinearHashTable *table, int PIN);
size_t DeleteHashTable(LinearHashTable *table);
void PrintHashTable(LinearHashTable *table);
int GetCapacity(LinearHashTable *table);

#endif  // LINEAR_HASH_TABLE_H
