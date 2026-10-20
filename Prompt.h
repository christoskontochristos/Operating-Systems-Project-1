
#ifndef PROMPT_H
#define PROMPT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "linear_hash_table.h"
#include "reverse_index.h"
#include "Elector.h"


//the amount of words each command has (single sommands not included)
const int l_words = 2;
const int i_words = 5;
const int m_words = 2;
const int bv_words = 2;
const int z_words = 2;

const int MAX_WORDS = 5; //the maximum amount of words the input must have
const int MAX_PROMPT_INPUT = 255; //this is the maximum input a user can give to the prompt

//Prompt class
class Prompt{
    private:
        bool flag; //flag that determines when the prompt loop ends
        char *userinput; //the user input
    public: 
        //class methods prototypes 
        Prompt();
        size_t DeletePrompt();
        char* ExtractCommand();
        int IsInt(char *string);
        int IsAtoZstring(char *string);
        int CheckPlainCommand();
        int CountWords();
        char **ExtractWords();
        int CheckForEnterPress();
        void Command_m(struct LinearHashTable *table,int PIN,struct ReverseIndex *ri);
        void Command_i(struct LinearHashTable *table,int PIN,char *lname,char *fname,int PC);
        void Command_l(struct LinearHashTable *table,int PIN);
        void Command_o(struct ReverseIndex *ri);
        void Command_z(struct ReverseIndex *ri,int zip);
        void Command_v(struct ReverseIndex *ri);
        void Command_perc(struct LinearHashTable *table,struct ReverseIndex *ri);
        void Command_bv(struct LinearHashTable *table,char *file,struct ReverseIndex *ri);
        void Command_t(struct LinearHashTable *table);
        void PromptLoop(struct LinearHashTable *table,struct ReverseIndex *ri);
};

#endif