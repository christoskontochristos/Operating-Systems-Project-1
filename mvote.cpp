#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "linear_hash_table.h"
#include "reverse_index.h" 
#include "InputFile.h"
#include "Prompt.h"

static const char * FILE_FLAG = "-f";
static const char * BUCKET_ENTRIES_FLAG = "-b";
static const char * INITIAL_BUCKETS_NUM = "-m";



//utility function to check if a string is an integer
bool IsInt(char *string){
    int len = strlen(string);
    int count = 0;
    for(int i=0;i<len;i++){
        if(isdigit(string[i])) count++;
    }
    if(count == len) return true;
    return false;
}

//main function
int main(int argc,char *argv[]){
    //parse the command line arguments
    int b,m;
    char *file;
    if(argc < 7){ 
        printf("ERROR: Too Few Commands!\n"); 
        exit(1);
    }
    for(int i=0;i<argc;i++){
        if(strcmp(argv[i],FILE_FLAG)==0){ //check for the -f flag
            printf("file : %s\n",argv[i+1]);
            file = argv[i+1];
        }
        if(strcmp(argv[i],BUCKET_ENTRIES_FLAG)==0){ //check for the -b flag
            if(IsInt(argv[i+1]) == false){  //check if the argument of the -b flag is an integer
                printf("ERROR: The argument of the -b flag  be an integer!\n");
                exit(1);
            }
            else{ 
                b = atoi(argv[i+1]);
                printf("Bucket size : %d\n",b); 
            }
        }
        if(strcmp(argv[i],INITIAL_BUCKETS_NUM)==0){ //check for the -m flag
            if(IsInt(argv[i+1]) == false){  //check if the argument of the -m flag is an integer
                printf("ERROR: The argument of the -m flag must be an integer!\n");
                exit(1);
            }
            else{ 
                m = atoi(argv[i+1]);
                printf("Bucket num : %d\n",m); 
            }
        }
    }
    InputFile inf(file);  //create an InputFile class object
    struct LinearHashTable * table = InitHashTable(b,m); //create and initialize the hash table
    struct ReverseIndex *ri = Initialize(); //create and initialize the reverse index
    inf.ReadInputFile(table); //read the input file and insert everything to the hash table 
    Prompt prompt; //create a Prompt class object 
    prompt.PromptLoop(table,ri); //start the prompt loop till the program finishes
    return 0;
}