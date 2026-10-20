#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stddef.h>
#include "Elector.h"
#include "reverse_index.h"

//utility function to create a new Node for the inner Electors linked list
//time complexity: O(1)
struct ElectorsListNode * Utility(Elector *elector){
    struct ElectorsListNode * new_node = (struct ElectorsListNode *)malloc(sizeof(struct ElectorsListNode));
    if(new_node == NULL){ 
        printf("Memory not allocated\n");
        exit(1);
        return NULL;
    }
    new_node->elector = elector;
    new_node->next = NULL;
    return new_node;
}

//insert an elector to the inner linked list of Electors
//time complexity: O(n) where n is the number of the electors in the list
void InsertElector(struct ElectorsListNode ** head,Elector * el){
    struct ElectorsListNode * new_elector = Utility(el); //create a new node for the list 
    //if the list is empty make the new node the head of the list
    if(*head == NULL){
        *head = new_elector;
        return;
    }
    struct ElectorsListNode * temp = *head;
    //iterate until the end of the list to insert the new node
    while(temp->next != NULL){
        temp = temp->next;
    }
    temp->next = new_elector;
    return;
}

//print the inner linked list of the Electors
//time complexity: O(n) where n is the number of the electors in the list
void PrintElectorList(struct ElectorsListNode * head){
    if(head == NULL){
        printf("The list is empty!\n");
        return;
    }
    struct ElectorsListNode * temp = head;
    while(temp->next != NULL){
        printf("PIN : %d|",temp->elector->GetPin());
        printf("First Name : %s|",temp->elector->GetName());
        printf("Last Name : %s|",temp->elector->GetLastname());
        printf("Postal Code : %d|",temp->elector->GetPC());
        printf("Status : %c|\n",temp->elector->GetStatus());
        temp = temp->next;
    }    
    printf("PIN : %d|",temp->elector->GetPin());
    printf("First Name : %s|",temp->elector->GetName());
    printf("Last Name : %s|",temp->elector->GetLastname());
    printf("Postal Code : %d|",temp->elector->GetPC());
    printf("Status : %c|\n",temp->elector->GetStatus());
    return;
}

//utility function to Create a new Reverse Index Node
//time complexity: O(1)
struct RevIdxNode * CreateNode(int PC){
    struct RevIdxNode * new_node = (struct RevIdxNode *)malloc(sizeof(struct RevIdxNode));
    if(new_node == NULL){ 
        printf("Memory not allocated\n");
        exit(1);
        return NULL;
    }
    new_node->PostalCode = PC;
    new_node->list_head = NULL;
    new_node->next = NULL;
    new_node->size = 0;
    return new_node;
}

//function to Initialize the Reverse Index data structure 
//time complexity: O(1)
struct ReverseIndex * Initialize(){
    struct ReverseIndex * revidx = (struct ReverseIndex *)malloc(sizeof(struct ReverseIndex));
    if(revidx == NULL){ 
        printf("Memory not allocated\n");
        exit(1);
        return NULL;
    }
    revidx->head = NULL;
    revidx->capacity = 0;
    return revidx;
}

//Insert a new Elector to the structure 
//time complexity: O(m*n) where m is the number of Reverse index Nodes and n is the max number of electors in each node
void Insert(struct ReverseIndex * revidx,Elector *elector){
    //case when the reverse index is empty 
    if(revidx->head == NULL){
        struct RevIdxNode *new_node = CreateNode(elector->GetPC()); //Create a new reverse index node
        revidx->head = new_node; //make that node the head of the reverse index
        InsertElector(&(new_node->list_head),elector); //insert the elector
        new_node->size++;
        revidx->capacity++;
        return;
    }
    //the case when the reverse index is not empty
    //iterate through the reverse index until you find the node with the according PC
    struct RevIdxNode * temp = revidx->head;
    while(temp->next != NULL){
        //if the node is found insert the elector and return
        if(temp->PostalCode == elector->GetPC()){ 
            InsertElector(&(temp->list_head),elector);
            temp->size++;
            revidx->capacity++;
            return;
        }
        temp = temp->next;
    }
    if(temp->PostalCode == elector->GetPC()){
        InsertElector(&(temp->list_head),elector);
        temp->size++;
        revidx->capacity++;
        return;
    }
    //the case when the PC doesnt exist in the reverse index -> insert a new reverse index node
    struct RevIdxNode *new_node = CreateNode(elector->GetPC()); //create a new Reverse Index Node
    new_node->size++;
    temp->next = new_node;
    revidx->capacity++;
    InsertElector(&(new_node->list_head),elector); //Insert the Elector
    return;
}

//BubbleSort algorithm function to sort the Reverse Index according to the amount of Electors in each node
//time complexity O(n*n) where n is the number of the electors in the Electors list
void BubbleSort(struct ReverseIndex *ri){
    if(ri->head!=NULL){
        struct RevIdxNode *current = NULL;
        struct RevIdxNode *new_head = NULL;
        struct RevIdxNode *move_node = NULL;
        struct RevIdxNode *prev = NULL;
        //iterate through the list until the head becomes NULL
        while(ri->head != NULL){
            prev = NULL;
            current = ri->head;
            move_node = ri->head;
            //find the node with the maximum amount of electors
            while(current != NULL){
                if(current->next != NULL &&  current->next->size < move_node->size){
                    //if the next node has a greater size, update move_node and prev
                    move_node = current->next;
                    prev = current;
                }
                current = current->next;
            }
            if(move_node == ri->head){
                ri->head = ri->head->next;
            }
            if(prev != NULL){
                prev->next = move_node->next;
            }
            move_node->next = new_head;
            new_head = move_node;
        }
        //make the head of the reverse index point to the sorted reverse index
        ri->head = new_head;
    }else{
        printf("The reverse index is empty!\n");
    }
}

//function to delete the Reverse Index data structure and free all the allocated memory
//also returns the amount of bytes that have been freed
//time complexity: O(m*n) where m is the number of Reverse Index Nodes / n is the number of Electors in the electors list
size_t DeleteReverseIndex(struct ReverseIndex *revidx){
    size_t bytes_counter = 0;
    if (revidx == NULL){ //the case where the reverse index is empty
        printf("Reverse Index is empty!\n");
        return 0;
    }
    struct RevIdxNode *temp = revidx->head;
    struct RevIdxNode *next;
    //iterate through the reverse index and free all the allocated memory
    while(temp != NULL){
        struct ElectorsListNode *list = temp->list_head;
        struct ElectorsListNode *enext;
        while(list != NULL){
            enext = list->next;
            bytes_counter += sizeof(struct ElectorsListNode) + sizeof(struct Elector);
            free(list->elector);
            free(list);
            list = enext;
        }
        next = temp->next;
        bytes_counter += sizeof(struct RevIdxNode);
        free(temp);
        temp = next;
    }
    bytes_counter += sizeof(struct ReverseIndex);
    free(revidx);
    return bytes_counter;
}

//function to print the reverse index 
//time complexity: O(m*n) where m is the number of Reverse Index Nodes / n is the number of Electors in the electors list
void PrintReverseIndex(struct ReverseIndex * revidx){
    struct RevIdxNode * temp = revidx->head;
    if(temp == NULL) return;
    while(temp->next != NULL){
        printf("Postal Code: %d\n",temp->PostalCode);
        PrintElectorList(temp->list_head);
        printf("\n\n");
        temp = temp->next;
    }
    printf("Postal Code: %d\n",temp->PostalCode);
    PrintElectorList(temp->list_head);
    return;
}

//getters for information hiding
//time complexity: O(1)
int GetCapacity(struct ReverseIndex *ri){ return ri->capacity; }
int GetNodeSize(struct RevIdxNode *node){ return node->size; }
int GetPostalCode(struct RevIdxNode *node){ return node->PostalCode; }
struct RevIdxNode * GetRiHead(struct ReverseIndex *ri){ return ri->head; }
struct RevIdxNode * GetNext(struct RevIdxNode *node){ return node->next; }
struct ElectorsListNode *GetListHead(struct RevIdxNode *node){ return node->list_head; }
struct ElectorsListNode *GetListNodeNext(struct ElectorsListNode *node){ return node->next; }
Elector *GetElector(struct ElectorsListNode *node){ return node->elector; }

