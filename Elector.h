#ifndef ELECTOR_H
#define ELECTOR_H

const int MAX_NAME_LENGTH = 255;

class Elector{
    private:
        int PIN;
        char firstname[MAX_NAME_LENGTH];
        char lastname[MAX_NAME_LENGTH];
        int PC;
        char status;
    public:
        Elector(int PIN, char* firstname, char* lastname, int PC);
        int GetPin();
        char* GetName();
        char* GetLastname();
        int GetPC();
        char GetStatus();
        void SetStatus(char status);
        ~Elector(); // destructor
};

#endif
