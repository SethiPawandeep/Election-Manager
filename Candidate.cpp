#include "Candidate.h"
#include "cstring"

Candidate::Candidate(char* id, char* pass, char* constituency, char* address, int age):User(id, pass, false){
    strcpy(this->constituency, constituency);
    strcpy(this->address, address);
    this->age = age;
}
