#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "linear_hash_table.h"
#include "reverse_index.h"
#include "Elector.h"
#include "Prompt.h"

//constructor for the Prompt class
//the maximum prompt input is MAX_PROMPT_INPUT = 255
Prompt::Prompt(){ 
    this->flag = true;
    this->userinput = new char[MAX_PROMPT_INPUT]; 
    this->userinput[0] = '\0';
}

size_t Prompt::DeletePrompt(){
    delete[] this->userinput;
    size_t bytes_freed = 0;
    bytes_freed += sizeof(char) * MAX_PROMPT_INPUT;
    return bytes_freed;
}

//function to extract/get the first word of the input string which is the command
char* Prompt::ExtractCommand(){ 
    char* command = new char[100];
    command[0] = '\0';
    int flag = 0; //flag to indicate if we have reached the first non-space/tab characters of the input string
    for(int i=0 ;i<strlen(this->userinput); i++){
        //when the commmand starts with space or tab characters ignore them
        if((this->userinput[i] == ' ' || this->userinput[i] == '\t') && flag == 0){ continue; } 
        //if flag = 1, and the character is space or tab then break the loop cause we have obtained the command
        //else -> get the command
        if(this->userinput[i] == ' ' || this->userinput[i] == '\t'){ 
            break; 
        }
        else{ 
            strncat(command, &(this->userinput[i]), 1); 
            flag = 1;
        }
    }
    return command;
}

//function to check if a string is an integer number
//returns 1 if it's int and 0 otherwise
int Prompt::IsInt(char *string){
    int len = strlen(string);
    int count = 0;
    for(int i=0;i<len;i++){
        //the case where the string ends in '\0' or new line 
        if(i == len-1 && (string[i] == '\n' || string[i] == '\0' || string[i] == '\r')) count++; 
        if(isdigit(string[i])) count++; //check if every character of the given string is a digit
    }
    if(count == len) return 1;
    return 0;
}

//function to check if a string only consists of A-Z capital letters
//returns 1 if true and 0 if false
int Prompt::IsAtoZstring(char *string){
    int len = strlen(string);
    for(int i=0;i<len;i++){
        if(string[i] == '\0') break;  //if the current char is '\0' means that we are at the end of the string
        //if(string[i] == ' ' || string[i] == '\t') continue; // maybe remove this
        if(string[i]<65 || string[i]>90) return 0; 
    }
    return 1;            
}


//function to check if a plain/single command (a command with no flags or arguments) is valid
//returns 1 if it's valid and 0 if not
int Prompt::CheckPlainCommand(){ 
    int flag = 0; //flag to indicate if we are at the last character of the command
    for(int i=0 ;i<strlen(this->userinput); i++){
        if(flag == 1){ 
        //if we have reached the end of the command and the next characters are not space/null/tab/new line then the command is not valid 
            if((this->userinput)[i] != '\0' && (this->userinput)[i] != ' ' && (this->userinput)[i] != '\n' && (this->userinput)[i] != '\t' && (this->userinput)[i] != '\r'){
                return 0;
            }
        }
        //if we haven't reached the command then ignore the space and tab characters
        if((this->userinput)[i] == ' ' || (this->userinput)[i] == '\t'){ continue; }
        //if the current char is not space, and the next one is not \0 or \t or space that means we are at the last char of the command 
        if((this->userinput)[i] != ' ' && ((this->userinput)[i+1] == '\t' || (this->userinput)[i+1] == '\0' || (this->userinput)[i+1] == ' ')){ //that means that we are at the last character of the command
            flag = 1;
        }
    }
    return 1;
}

//function to count the words in the input string
//a word is the seqence of characters that are seperated by spaces or tabs
int Prompt::CountWords(){
    int flag = 0;
    int count = 0;
    for(int i = 0;i<strlen(this->userinput);i++){
        if((this->userinput)[i] == ' ' || (this->userinput)[i] == '\t' || (this->userinput)[i] == '\n') flag = 0;
        else if(flag == 0){
            flag = 1;
            count++;
        }
    }
    return count;
}

//function to extract the words of the input string and store them into an array of strings
//the max amount of words the input string can contain is MAX_WORDS = 5
char ** Prompt::ExtractWords(){
    char **word_array = new char*[MAX_WORDS]; //allocate memory for the array of strings
    int word_count = 0;
    char *token = strtok(this->userinput, " \t\n"); //get the first token
    while(token != NULL && word_count < MAX_WORDS){
        word_array[word_count] = strdup(token); //copy the token to the word_array
        word_count++;
        token = strtok(NULL, " \t\n"); //get the next token
    }
    return word_array;
}

//function to check if the user presses enter ('\n')
//or if the user presses only spaces or tabs and then enter
//returns 1 if true and 0 if false
int Prompt::CheckForEnterPress(){
    //if the first charaster of the input string is '\n' then the user only pressed enter
    if(this->userinput[0] == '\n' || this->userinput[0] == '\r') return 1; 
    //iterate through all the characters in the string besides the last one and if you find a char thas is not space or tab return 0
    for(int i = 0;i<strlen(this->userinput)-1;i++){
        if(this->userinput[i] != ' ' && this->userinput[i] != '\t') return 0;
    }
    //if all the chars in the string are spaces or tabs and the last one is '\n' return 1
    if(this->userinput[strlen(this->userinput)-1] == '\n') return 1;
    else return 0;
    return 0;
}

//function to execute the l command
void Prompt::Command_l(struct LinearHashTable *table,int PIN){
    Elector * el = Search(table,PIN); //search for the pin in the hash table
    if(el == NULL){
        printf("Participant %d is not in cohort!\n",PIN);
    }
    else{
        printf("%d ",el->GetPin());
        printf("%s ",el->GetLastname());
        printf("%s ",el->GetName());
        printf("%d ",el->GetPC());
        printf("%c",el->GetStatus());
        printf("\n");
    }
    return;
}

//function to execute the m command
void Prompt::Command_m(struct LinearHashTable *table,int PIN,struct ReverseIndex *ri){   
    //search for an elector in the hash table and if the elector exists make their status "Y"
    Elector * el = Search(table,PIN);
    if(el == NULL){
        printf("%d does not exist!\n",PIN);
    }
    else{
        if(el->GetStatus() == 'Y'){
            printf("Elector with pin : %d has already voted\n",el->GetPin());
            return;
        }
        el->SetStatus('Y');
        printf("%d Marked Voted.\n",PIN);
        Insert(ri,el);
    }
}

//function to execute the o command
void Prompt::Command_o(struct ReverseIndex *ri){
    BubbleSort(ri); //sort the reverse index
    //iterate through the reverse index and print the data
    struct RevIdxNode *temp = GetRiHead(ri);    
    while(temp != NULL){
        if(GetNodeSize(temp) == 0) continue;
        else{
            printf("%d  %d\n",GetPostalCode(temp),GetNodeSize(temp));  
        }
        temp = GetNext(temp);
    }
    return;
}

//function to execute the z command
void Prompt::Command_z(struct ReverseIndex *ri,int zip){
    if(GetRiHead(ri) == NULL){    //if the head of the reverse index is NULL return
        printf("The list is empty");
        return;
    }
    struct RevIdxNode *temp = GetRiHead(ri); 
    int flag = 0;
    //iterate through the reverse index  
    while(temp != NULL){
        //if the given PC is found then iterate through the Electors linked list 
        //of that current RI node and print the data
        if(GetPostalCode(temp) == zip){ 
            flag = 1;
            printf("%d voted in %d\n",GetNodeSize(temp),GetPostalCode(temp));   
            struct ElectorsListNode *temp1 = GetListHead(temp);    
            while(GetListNodeNext(temp1) != NULL){
                printf("%d\n",GetElector(temp1)->GetPin());
                temp1 = GetListNodeNext(temp1);
            }
            printf("%d\n",GetElector(temp1)->GetPin());
        }
        temp = GetNext(temp);
    }
    if(flag == 0) printf("The given zipcode does not exist in the reverse index\n");
}

//function to execute the i command
void Prompt::Command_i(struct LinearHashTable *table,int PIN,char *lname,char *fname,int PC){
    //if the Pin is not already in the table then create a pointer to an Electors class object
    //and insert it to the hash table
    if(Search(table,PIN) != NULL){
        printf("%d already exists in the hash table!\n",PIN);
    }
    else{
        Elector * new_elector = new Elector(PIN,fname,lname,PC);
        InsertKey1(table,new_elector);
    }
    return;
}

//function to execute the v command 
void Prompt::Command_v(struct ReverseIndex *ri){
    //if the Reverse Index head is NULL then zero electors have voted
    if(ri == NULL) printf("Voted So Far 0\n"); 
    else printf("Voted So Far %d\n",GetCapacity(ri)); 
}

//utility function to change the voting status of an elector
//will be used for the bv command
void SetVotingStatus(struct LinearHashTable *table,int PIN,struct ReverseIndex *ri){
    //search the hash table and check if the pin exists or not
    Elector * el = Search(table,PIN);
    if(el == NULL){
        printf("%d does not exist!\n",PIN); 
    }
    else{
        if(el->GetStatus() == 'Y'){  //if the elector with the given pin has already voted return
            printf("Elector with pin %d has already voted\n",el->GetPin());
            return;
        }
        el->SetStatus('Y'); //set the voting status to 'Y'
        Insert(ri,el); //Insert it in the reverse index data structure 
    }
}

//function to execute the bv commmand
void Prompt::Command_bv(struct LinearHashTable *table,char *file,struct ReverseIndex *ri){ 
    FILE *fptr = fopen(file,"r"); //open the file that contains the pins
    if(fptr == NULL){ 
        printf("%s could not be opened!\n",file);
        return;
    }
    int buffer_length = 20;  //set the buffer length for the fgets()
    char buffer[buffer_length];  // teh buffer
    while(fgets(buffer,buffer_length,fptr)){   //get each line of the file using fgets()
        char pin[buffer_length];  //array to store the pin string 
        for(int i=0;i<strlen(buffer);i++){ //get the pin string
            if(buffer[i] == ' ' || buffer[i] == '\t' || buffer[i] == '\n' || buffer[i] == '\0') continue;
            else{
                pin[i] = buffer[i];
            }
        }
        if(IsInt(pin) == 0){ //check if the string is an integer and continue if its not 
            printf("Malformed Input!\n");
            continue;
        }
        int PIN = atoi(pin); //convert it to int
        SetVotingStatus(table,PIN,ri);  //change the voting status
    }
    fclose(fptr); //close the file
    return;
}

//function to execute the perc command
void Prompt::Command_perc(struct LinearHashTable *table,struct ReverseIndex *ri){ 
    if(GetCapacity(table) != 0){ 
        printf("%f\n",(double)(GetCapacity(ri))/(GetCapacity(table)));
    }
    else{
        printf("The table is empty!\n");
    }
    return;
}

//function to execute the t command
//the t command is a command that takes 0 args/flags 
//and prints the total number of voters in the hash table
void Prompt::Command_t(struct LinearHashTable *table){
    if(table == NULL) printf("Total Voters : 0\n");
    else printf("Total Voters : %d\n",table->capacity);
}

//the Prompt Loop
//loops until the loop flag becomes false which means when the user types the exit command
//then it extracts the command from the input string and checks which command the user typed
//if it is a single command (a command with no arguments or flags e.g. 'exit') then it uses
//the CheckPlaiCommand() function to check if the command is valid
//if it is not a single command then it extracts all the words form the input string and
//firstly it checks if the number of words is right and then depending on the command
//it checks if the arguments are valid 
void Prompt::PromptLoop(struct LinearHashTable *table,struct ReverseIndex *ri){ 
    size_t byte_counter = 0;
    while(this->flag){
        printf("-->");
        fgets(this->userinput, MAX_PROMPT_INPUT, stdin); //get the user input 
        if(CheckForEnterPress() == 1){ //if the user presses enter OR presses many spaces/tabs and then enter
            continue;  //if true then continue
        }
        int len = strlen(this->userinput); //get the length of the input string
        if(len > 0 && this->userinput[len-1] == '\n'){
            this->userinput[len-1] = '\0';  //Replace newline with null character
        }
        char *command = ExtractCommand(); //extract the command of the input string (the command is always the first word of the input string)
        if(strcmp(command, "l") == 0){  //check if the user typed the l command 
            if(CountWords() == l_words){ //check if the number of words is right
                char **words = ExtractWords(); //extract the words from the input string
                //check if all the flags are valid
                if(IsInt(words[1]) == 0){
                    printf("ERROR: Malformed pin!\nThe argument of the 'l' command must be an integer.\n");
                }
                else{
                    Command_l(table,atoi(words[1]));
                }
                //also free the words array
                //for (int i = 0; i < MAX_WORDS; ++i){
                    //free(words[i]);
                //}
                //delete[] words;
            }
            else{
                printf("ERROR: The 'l' command takes 1 argument : <pin> \n");
            }
        }
        else if(strcmp(command, "i") == 0){ //check if the user typed the i command
            if(CountWords() == i_words){ //check if the number of words is right
                char **words = ExtractWords(); //extract the words from the input string
                //check if all the flags are valid
                int flag = 0;
                if(IsInt(words[1]) == 0){
                    printf("ERROR: Malformed pin!\nThe first argument of the 'i' command must be an integer.\n");
                    flag = 1;
                }
                if(IsInt(words[4]) == 0){
                    printf("ERROR: Malformed zipcode!\nThe last argument of the 'i' command must be an integer.\n");
                    flag = 1;
                }
                if(IsAtoZstring(words[2]) == 0){
                    printf("ERROR: Malformed lname!\nThe second argument of the 'i' command must be an sting with capital letters.\n");
                    flag = 1;
                }
                if(IsAtoZstring(words[3]) == 0){ 
                    printf("ERROR: Malformed fname!\nThe third argument of the 'i' command must be an string with capital letters.\n");
                    flag = 1;
                }
                if(flag == 1) continue;
                Command_i(table,atoi(words[1]),words[3],words[2],atoi(words[4]));
                //for (int i = 0; i < MAX_WORDS; ++i){
                    //free(words[i]);
                //}
                //delete[] words;
            }
            else{
                printf("ERROR: The 'i' command takes 4 arguments : <pin> <lname> <fname> <zip> \n");
            }
        }
        else if(strcmp(command, "m") == 0){ //check if the user typed the m command
            if(CountWords() == m_words){  //check if the number of words is right
                char **words = ExtractWords(); //extract the words from the input string
                //check if all the flags are valid
                if(IsInt(words[1]) == 0){
                    printf("ERROR: Malformed pin!\nThe argument of the 'm' command must be an integer.\n");
                }
                else{
                    Command_m(table,atoi(words[1]),ri); 
                }
                //for (int i = 0; i < MAX_WORDS; ++i){
                    //free(words[i]);
                //}
                //delete[] words;
            }
            else{
                printf("ERROR: The 'm' command takes 1 argument : <pin>\n");
            }
        }
        else if(strcmp(command, "bv") == 0){ //check if the user typed the bv command
            if(CountWords() == bv_words){ //check if the number of words is right
                char **words = ExtractWords(); //extract the words from the input string
                Command_bv(table,words[1],ri);
                //for (int i = 0; i < MAX_WORDS; ++i){
                    //free(words[i]);
                //}
                //delete[] words;
            }
            else{
                printf("The 'bv' command takes 1 argument : <fileofkeys>\n");
            }
        }
        else if(strcmp(command, "v") == 0){ //check if the user typed the v command
            if(CheckPlainCommand() == 0){ //check if the command is valid
                printf("Wrong Command! If you want the sum of voted participants type 'v'.\n");
                continue; 
            }
            Command_v(ri);
        }
        else if(strcmp(command, "perc") == 0){ //check if the user typed the perc command
            if(CheckPlainCommand() == 0){ //check if the command is valid
                printf("Wrong Command! If you want the percentage of the voted participants type 'perc'.\n");
                continue; 
            }
            Command_perc(table,ri);
        }
        else if(strcmp(command, "z") == 0){ //check if the user typed the z command
            if(CountWords() == z_words){ //check if the number of words is right
                char **words = ExtractWords(); //extract the words from the input string
                //check if all the flags are valid
                if(IsInt(words[1]) == 0){
                    printf("ERROR: Malformed pin!\nThe argument of the 'z' command must be an integer.\n");
                }
                else{
                    Command_z(ri,atoi(words[1])); 
                }
                //for (int i = 0; i < MAX_WORDS; ++i){
                    //free(words[i]);
                //}
                //delete[] words;
            }
            else{
                printf("The 'z' command takes 1 argument : <zipcode>\n");
            }
        } 
        else if(strcmp(command, "o") == 0){ //check if the user typed the o command
            if(CheckPlainCommand() == 0){ //check if the command is valid
                printf("Wrong Command! If you want every zipcode in descending order type 'o'.\n");
                continue; 
            }
            Command_o(ri);
        }
        else if(strcmp(command, "pht") == 0){ //check if the user typed the pht command
            if(CheckPlainCommand() == 0){ //check if the command is valid
                printf("Wrong Command! If you want to print the hash table type 'pht'.\n");
                continue; 
            }
            printf("\n");
            PrintHashTable(table);
        }
        else if(strcmp(command, "pri") == 0){ //check if the user typed the pri command
            if(CheckPlainCommand() == 0){ //check if the command is valid
                printf("Wrong Command! If you want to print the hash table type 'pri'.\n");
                continue; 
            }
            printf("\n");
            PrintReverseIndex(ri);
        }
        else if(strcmp(command, "t") == 0){ //check if the user typed the t command
            if(CheckPlainCommand() == 0){ //check if the command is valid
                printf("Wrong Command! If you want the total number of the electors type 't'.\n");
                continue; 
            }
            Command_t(table);
        }                                
        else if(strcmp(command, "exit") == 0){ //check if the user typed the exit command
            if(CheckPlainCommand() == 0){ //check if the command is valid
                printf("Wrong Command! If you want to exit type 'exit'.\n");
                continue; 
            }
            printf("\n");
            this->flag = false; //set the loop flag to false so the prompt loop stops and the program finishes
            //Before exiting count the bytes freed
            byte_counter += DeleteHashTable(table);
            byte_counter += DeleteReverseIndex(ri);
            byte_counter += DeletePrompt();
            printf("%zu bytes freed\n",byte_counter);
            printf("exiting...\n");
        }
        else{ printf("ERROR: Command not found.\n"); }  //if the command its not found
    }
}

