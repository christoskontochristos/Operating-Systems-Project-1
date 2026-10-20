
#ifndef INPUTFILE_H
#define INPUTFILE_H

#include <stdio.h>
#include "Elector.h"
#include "linear_hash_table.h"

const int BUFFER_LENGTH = 255; //the buffer length for every file line
const int max_words = 4; //the maximum amount of words for each file line 

//InputFile class
class InputFile{
    private:
        char *file; //the file 
        FILE *fptr; //FILE pointer 
    public:
        //class methods prototypes 
        InputFile(char *file);
        char **ExtractWords(char buffer[BUFFER_LENGTH]);
        void ReadInputFile(struct LinearHashTable *table);
        ~InputFile(); //destructor
};

#endif