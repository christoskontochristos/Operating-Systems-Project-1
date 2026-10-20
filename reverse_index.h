
#ifndef REVERSE_INDEX_H
#define REVERSE_INDEX_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stddef.h>
#include "Elector.h"


static size_t bytes_freed; //global variable to count the bytes that have been freed

//the ElectorsListNode struct which is the inner linked list for every reverse index node
struct ElectorsListNode{
    Elector *elector; //pointer to an Elector class object
    struct ElectorsListNode *next; //pointer to the next node
};

//the RevIdxNode struct is a struct to represent a reverse index Node
struct RevIdxNode{
    int PostalCode; //the postal code 
    int size; //the size of the inner linked list 
    struct ElectorsListNode *list_head; //pointer to the head of the inner linked list 
    struct RevIdxNode *next; // pointer to the next reverse index Node
};

//the ReverseIndex struct 
struct ReverseIndex{
    struct RevIdxNode * head; //pointer to the head of the Reverse Index data structure 
    //int size; //the amount of Nodes the reverse index has 
    int capacity; //the amount Electors 
};

//function prototypes
struct ReverseIndex * Initialize();
void Insert(struct ReverseIndex * revidx,Elector *elector);
void BubbleSort(struct ReverseIndex *ri);
size_t DeleteReverseIndex(struct ReverseIndex *revidx);
void PrintReverseIndex(struct ReverseIndex * revidx);
int GetCapacity(struct ReverseIndex *revidx);
int GetNodeSize(struct RevIdxNode *node);
int GetPostalCode(struct RevIdxNode *node);
struct RevIdxNode * GetRiHead(struct ReverseIndex *ri);
struct RevIdxNode * GetNext(struct RevIdxNode *node);
struct ElectorsListNode *GetListHead(struct RevIdxNode *node);
struct ElectorsListNode *GetListNodeNext(struct ElectorsListNode*node);
Elector *GetElector(struct ElectorsListNode *node);

#endif