#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "InputFile.h"
#include "linear_hash_table.h"
#include "Elector.h"

//constructor for the InputFile class
InputFile::InputFile(char *file){
    this->file = file;
    this->fptr = NULL;
}

//utility function to extract the words from a string line in the file
char **InputFile::ExtractWords(char buffer[BUFFER_LENGTH]){
    char **words = new char*[max_words];
    if(words == NULL){
        printf("Memory allocation failed\n");
        exit(1);
        return NULL;       
    }
    int wordCount = 0;
    char *token = strtok(buffer, " \t\n");
    while(token != NULL && wordCount < max_words){
        words[wordCount] = strdup(token);
        wordCount++;
        token = strtok(NULL, " \t\n");
    }
    return words;
}

//function to read the input file, create a pointer to an Elector class object for each line of the file
//and insert them to the hash table
void InputFile::ReadInputFile(struct LinearHashTable *table){
    char buffer[BUFFER_LENGTH]; //buffer array for each line of the file
    this->fptr = fopen(this->file,"r"); //open the file 
    if(fptr == NULL){ //check if the file can be opened
        printf("ERROR: could not open file!\n");
        exit(1);
    }
    //get each line from the file using fgets
    while(fgets(buffer,BUFFER_LENGTH,this->fptr)){
        char **words = ExtractWords(buffer); //extract the words from the file line
        int PIN = atoi(words[0]); //convert the first words which is the pin to an integer 
        char *fname = words[2]; 
        char *lname = words[1];
        int PC = atoi(words[3]); //convert the second word which is the PC to an integer
        Elector * new_elector = new Elector(PIN,fname,lname,PC); //create the pointer to the Electors class object
        InsertKey1(table,new_elector); //insert the elector to the table 
        for(int i = 0; i < max_words; ++i){ //free the words array
            free(words[i]);
        }
        delete [] words;
    }
    fclose(this->fptr); //close the file
}

InputFile::~InputFile(){} //destructor
