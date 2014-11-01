#include "Candidate.h"
#include "cstring"

Candidate::Candidate(char* id,char* constituency, char* address, int age){
    strcpy(this->constituency, constituency);
    strcpy(this->address, address);
    this->age = age;
    strcpy(this->id,id);
}
