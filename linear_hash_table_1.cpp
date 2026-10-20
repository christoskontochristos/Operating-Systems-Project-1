#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stddef.h>
#include <unistd.h>
#include "Elector.h"
#include "linear_hash_table.h"


//utility function to Create a new bucket
//time complexity: O(bucket_size) 
struct Bucket * Bucket_Utillity(int idx,int bucket_size){
    struct Bucket *new_bucket = (struct Bucket*)malloc(sizeof(struct Bucket));
    new_bucket->Array = (Elector**)malloc(bucket_size*sizeof(Elector*));
    if(new_bucket->Array == NULL){ 
        printf("Memory not allocated\n");
        exit(1);
        return NULL;
    }
    for(int i=0;i<bucket_size;i++){
        new_bucket->Array[i] = NULL;
    }
    new_bucket->index_number = idx;
    new_bucket->next = NULL;
    return new_bucket;
}

//function to insert a bucket to the linked list of buckets (basically an overflow bucket)
//time complexity: O(n) / n is the number of buckets in the bucket list 
void InsertBucket(struct Bucket **ptr,int idx,int bucket_size){
    struct Bucket *new_bucket = Bucket_Utillity(idx,bucket_size);
    if(*ptr == NULL){
        *ptr = new_bucket;
        return;
    }
    struct Bucket *temp = *ptr;
    while(temp->next != NULL){
        temp = temp->next;
    }
    temp->next = new_bucket;
}

//print the Bucket Row - The linked list of buckets
//time complexity: O(n) / n is the number of buckets in the bucket list 
void PrintBucketRow(struct Bucket *ptr,int bucket_size){
    struct Bucket *temp = ptr;
    while(temp != NULL){
        for(int i=0;i<bucket_size;i++){
            if(temp->Array[i] == NULL) printf("[]");
            else printf("[%d]",temp->Array[i]->GetPin());
        }
        if(temp->next != NULL) printf("-->");
        temp = temp->next;
    }
}

//function to initialize the hash table
//time complexity: O(bucket_size*bucket_num) 
struct LinearHashTable *InitHashTable(int bucket_size,int bucket_num){
    struct LinearHashTable *table = (struct LinearHashTable *)malloc(sizeof(struct LinearHashTable));
    if(table == NULL){
        printf("Memory allocation failed\n");
        exit(1);
        return NULL;       
    }
    table->array = (struct Bucket**)malloc(bucket_num*sizeof(struct Bucket));
    if(table->array == NULL){
        printf("Memory allocation failed\n");
        exit(1);
        return NULL;       
    }
    int i;
    for(i = 0;i<bucket_num;i++){
        table->array[i] = Bucket_Utillity(i,bucket_size);
    }
    table->pointer = table->array[0];
    table->current_size = bucket_num;
    table->round = 0;
    table->capacity = 0;
    table->m = bucket_num;
    table->round_count = 1;
    table->bucket_size = bucket_size;
    return table;
}  

//function to get the load factor
//time complexity: O(1) 
double LoadFactor(int capacity,int bucket_num){ return (double)capacity / bucket_num; }

//the hash function
//time complexity: O(1) 
int HashFunction(int key,int i,int m){  
    return key % ((1 << i) * m);
}

//function to move the pointer to the bucket to be split in the first bucket of the table
//time complexity: O(1) 
void MovePointerAtFront(struct LinearHashTable *table){ table->pointer = table->array[0]; }

//function to Insert an Overflow Bucket
//time complexity: O(n) / n is the number of buckets in the bucket list 
void InsertOverflowBucket(struct LinearHashTable *table,int idx){
    //check if the table size is smaller than the index
    if(idx > table->current_size-1){
        printf("ERROR: table size is smaller than the index\n");
        return;
    }
    struct Bucket *temp = table->array[idx];
    //table->ofbuckets_size++;
    InsertBucket(&temp,idx,table->bucket_size); //insert a new bucket at the linked list of buckets
}

//function to Insert a new bucket at the table (after splitting)
//time complexity: O(bucket_size) cause of Bucket_Utillity()
void InsertNewBucket(struct LinearHashTable *table){
    int size = table->current_size;
    struct Bucket ** newarray;
    //reallocate memory for the new table array and make it have one more bucket
    newarray = (struct Bucket **)realloc(table->array,(size+1)*sizeof(struct Bucket));
    if (newarray == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
        return;
    }
    table->array = newarray;
    table->array[size] = Bucket_Utillity(size,table->bucket_size);
    table->current_size++;
}

//function to place a key (a pointer to an Electors class object) to a bucket according the hash
//time complexity: O(n) / n is the number of buckets in the bucket list 
void AddKey(Elector *elector,int hash,struct LinearHashTable *table){
    int size = table->current_size;
    //check if the hash is bigger than the table size
    if(hash > (size-1)){
        printf("ERROR: TABLE SIZE IS SMALLER THAN THE HASH\n");
        return;
    }
    //get the bucket row according to the hash
    struct Bucket *temp = table->array[hash];
    //iterate through the list of buckets until a free spot is found
    while(temp != NULL){
        for(int i=0;i<table->bucket_size;i++){
            if(temp->Array[i] == NULL){
                temp->Array[i] = elector;
                return;
            }
        }
        temp = temp->next;
    }
}

//utillity function to Delete a given bucket
//time complexity: O(1) 
void DeleteBucket(LinearHashTable * table,struct Bucket *bucket) {
    if(bucket == NULL){ return; } //if the pointer to the bucket is NULL the return
    bytes += (table->bucket_size)*sizeof(Elector); //update the byte counter
    free(bucket->Array);  //Free the memory of the array inside the bucket
    bytes += sizeof(struct Bucket); //update the byte counter
    free(bucket);  //Free the memory of the bucket itself
}

//function to Delete any empty overflow buckets after splitting
//time complexity: O(n) / n is the number of buckets in the bucket list 
void DeleteEmptyOverflowBuckets(LinearHashTable * table,struct Bucket *head){ 
    int ovf_count = 0; //counter to count how many overflow buckets this row of bucket has
    if(head == NULL){ return; } 
    struct Bucket *temp = head->next; 
    struct Bucket *prev = head->next; //pointer to the previous bucket 
    //iterate through the row of buckets 
    while(temp != NULL){
        ovf_count+=1;
        int count = 0;
        //check if every place at the bucket is NULL/empty
        for(int i=0;i<table->bucket_size;i++){
            if(temp->Array[i] == NULL) count+=1;
        }
        if(count == table->bucket_size){
            struct Bucket *temp1 = temp; //store the bucket to be deleted to a new temporary variable
            //if the bucket to be deleted is the first overflow bucket
            //then make the head->next of the bucket list point to NULL
            //cause that means every one of the next ovf buckets will be deleted as well
            //else make the previous bucket point to NULL
            if(ovf_count == 1){
                head->next = NULL;
            }
            else{
                prev->next = NULL;
            }
            DeleteBucket(table,temp1);
        }
        prev = temp;
        temp = temp->next;
    } 
    return;
}

//function to check if an overflow bucket is needed
//returns 1 if an overflow bucket is needed and 0 if not
//time complexity: O(n+bucket_size) / n is the number of buckets in the bucket list 
int CheckOverflow(struct LinearHashTable * table,int hash){
    struct Bucket *temp = table->array[hash];
    while(temp->next != NULL){
        temp = temp->next;
    }
    int count = 0; //counter to check if every spot in the bucket is taken
    for(int i = 0;i<table->bucket_size;i++){
        if(temp->Array[i] != NULL){
            count++;
        }
    }
    //if every spot in the bucket is taken -> overflow bucket is needed
    if(count == table->bucket_size) return 1;
    return 0;
}

//function to check for a spot for the newly inserted key
//this function is used when the bucket has one or more overflow buckets
//and it is about to be splitted
//returns 1 if there is a spot available and 0 if not 
//time complexity: O(n+bucket_size) / n is the number of buckets in the bucket list
int CheckForSpot(struct LinearHashTable * table,int hash){
    struct Bucket *temp = table->array[hash];
    int flag = 0;
    //iterate through the list of buckets and check if there is a spot available 
    while(temp != NULL){
        for(int i=0;i<table->bucket_size;i++){ 
            if(temp->Array[i] == NULL){
                flag = 1;
            }
        }
        temp = temp->next;
    }
    return flag;
}

//function to split a bucket that has one or more overflow buckets
//time complexity: O(n*bucket_size) / n is the number of buckets in the bucket list
void SplitOverflowBucket1(struct LinearHashTable * table){
    InsertNewBucket(table);  //insert a new bucket to the table 
    int index = table->pointer->index_number;
    struct Bucket *temp = table->array[index];
    //iterate through the list of buckets (the bucket according to the hash and its overflow buckets)
    //and redistribute all the keys using hash function 2
    while(temp != NULL){
        for(int i=0;i<table->bucket_size;i++){
            if(temp->Array[i] != NULL){
                int hash2 = HashFunction(temp->Array[i]->GetPin(),table->round+1,table->m);
                Elector * key = temp->Array[i];
                temp->Array[i] = NULL; //make that spot free
                //check if there is a spot available 
                //else Insert an overflow bucket
                if(CheckForSpot(table,hash2) == 0){
                    InsertOverflowBucket(table,hash2);
                    AddKey(key,hash2,table);
                }
                else{
                    AddKey(key,hash2,table);
                }
            }
        }
        temp = temp->next;
    }
    DeleteEmptyOverflowBuckets(table,table->array[index]);  //delete any empty buckets after the split 
    table->pointer = table->array[(table->pointer->index_number)+1]; //move the pointer to the next bucket of the table
}

//function to split the a bucket that has 0 overflow buckets
//time complexity: O(bucket_size)
void SplitBucket(struct LinearHashTable * table){
    int size = table->current_size;
    InsertNewBucket(table); //insert a new bucket
    int index = table->pointer->index_number;
    //iterate through the array of the bucket and redistribute the keys using hash function 2
    for(int i=0;i<table->bucket_size;i++){
        if(table->array[index]->Array[i] != NULL){
            Elector * key = table->array[index]->Array[i];
            int hash2 = HashFunction(key->GetPin(),table->round+1,table->m);
            table->array[index]->Array[i] = NULL;
            //check if an overflow is needed before placing the key
            if(CheckOverflow(table,hash2) == 1){
                InsertOverflowBucket(table,hash2);
                AddKey(key,hash2,table);
            }
            else{
                AddKey(key,hash2,table);
            }
        }
    }
    table->pointer = table->array[(table->pointer->index_number)+1]; //move the pointer to the next bucket of the table
}


//function to search the bucket list for the elector 
Elector * SearchBucketRow(struct LinearHashTable *table,int PIN,int hash){
    struct Bucket *temp = table->array[hash];
    //iterate through the bucket list and when the PIN is found return the elector
    while(temp != NULL){
        for(int i=0;i<table->bucket_size;i++){
            if(temp->Array[i] != NULL){
                if(temp->Array[i]->GetPin() == PIN) return temp->Array[i];
            }
        }
        temp = temp->next;
    }
    return NULL;
}

//the search function
//if the elector is not in the table then it returns NULL
//time complexity: almost O(1) 
Elector * Search(struct LinearHashTable *table,int PIN){
    //get the hash using hash function 1
    int hash1 = HashFunction(PIN,table->round,table->m);
    //if the hash is smaller than the index of the bucket that the pointer is pointing at uses hash function 2
    if(table->pointer->index_number > hash1){
        int hash2 = HashFunction(PIN,table->round+1,table->m);
        Elector *el = SearchBucketRow(table,PIN,hash2); //search the bucket list
        if(el != NULL){
            return el;
        }
    }
    else{
        Elector *el = SearchBucketRow(table,PIN,hash1); //search the bucket list
        if(el != NULL){
            return el;
        }
    }
    return NULL;
}

//Insert function
//time complexity: O(n*bucket_size) / n is the number of buckets in the bucket list
void InsertKey1(struct LinearHashTable * table,Elector *elector){
    //search the hash table to check if the pin already exists in the table
    if(Search(table,elector->GetPin()) != NULL){    
        printf("PIN already exists in the table!\n");
        return;
    }
    //get the hash1 using hash function 1
    int hash1 = HashFunction(elector->GetPin(),table->round,table->m); 
    //if the hash1 is smaller than the index of the bucket that the pointer points at, use hash function 2
    if(table->pointer->index_number > hash1){
        int hash2 = HashFunction(elector->GetPin(),table->round+1,table->m); 
        //Check for Overflow before placing the key to the table
        if(CheckOverflow(table,hash2) == 1){
            //if an overflow bucket is needed Insert a new overflow bucket
            InsertOverflowBucket(table,hash2);
            AddKey(elector,hash2,table);
            table->capacity++;
        }
        else{
            AddKey(elector,hash2,table);
            table->capacity++;
        }
    }
    else{
        //Check for Overflow before placing the key to the table
        if(CheckOverflow(table,hash1) == 1){
            //if an overflow bucket is needed Insert a new overflow bucket
            InsertOverflowBucket(table,hash1);
            AddKey(elector,hash1,table);
            table->capacity++;
        }
        else{
            AddKey(elector,hash1,table);
            table->capacity++;
        }
    }
    //get the load factor and check if spliting is required
    if(LoadFactor(table->capacity,table->current_size*table->bucket_size) > LOAD_FACTOR_THRESHOLD){
        int index = table->pointer->index_number;
        //check if the bucket to be split has one or more overflow buckets 
        if(table->array[index]->next != NULL){
            SplitOverflowBucket1(table);
        }
        else{
            SplitBucket(table);
        }
        //check for a new round after a split
        if(table->current_size == (table->m << table->round_count)){ 
            table->round++; 
            table->round_count++;
            MovePointerAtFront(table);
        }
    }
}


//function to delete the hash table and free all the allocated memory
//also returns the total amount of byted freed
//time complexity: O(n*m) / n is the max number of buckets in the bucket list/m is the length of the table array
size_t DeleteHashTable(struct LinearHashTable *table){ 
    if(table == NULL){
        printf("The table is empty!\n");
        return -1;
    }
    size_t bytes_counter = 0;
    for (int i = 0;i<table->current_size;i++) {
        struct Bucket *temp = table->array[i];
        while(temp != NULL){
            struct Bucket *next = temp->next;
            free(temp->Array); //free the array of the bucket 
            bytes_counter += sizeof(Elector *) * table->bucket_size;
            free(temp); //free the bucket
            bytes_counter += sizeof(struct Bucket);
            temp = next;
        }
    }
    bytes_counter += sizeof(struct Bucket ) * table->current_size; 
    free(table->array); //free the array of the table
    bytes_counter += sizeof(struct LinearHashTable);
    free(table); //free the table 
    bytes_counter += bytes;
    return bytes_counter;
}

//function to print the hash table
//time complexity: O(n*m) / n is the max number of buckets in the bucket list/m is the length of the table array
void PrintHashTable(struct LinearHashTable *table){
    int size = table->current_size;
    for(int i = 0;i<size;i++){
        printf("%d: ",table->array[i]->index_number);
        PrintBucketRow(table->array[i],table->bucket_size);
        if(table->pointer == table->array[i]){ //this is to print the pointer p (pointer to the bucket to be split)
            printf(" <- *p");
        }
        printf("\n");
    }
    printf("\n");
    return;
}

//getter for information hiding
//time complexity: O(1)
int GetCapacity(struct LinearHashTable *table){ return table->capacity; }