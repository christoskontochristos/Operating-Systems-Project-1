#include "Elector.h"
#include <string.h>


Elector::Elector(int PIN,char *firstname,char *lastname,int PC){ //constructor
    this->PIN = PIN;
    strncpy(this->firstname,firstname,MAX_NAME_LENGTH - 1);
    this->firstname[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(this->lastname,lastname,MAX_NAME_LENGTH - 1);
    this->lastname[MAX_NAME_LENGTH - 1] = '\0';
    this->PC = PC;
    this->status = 'N';
}

int Elector::GetPin(){ return this->PIN; }
char * Elector::GetName(){ return this->firstname; }
char * Elector::GetLastname(){ return this->lastname; }
int Elector::GetPC(){ return this->PC; }
char Elector::GetStatus(){ return this->status; }
void Elector::SetStatus(char status){ this->status = status; } 
Elector::~Elector(){} //destructor