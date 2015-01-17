#include "Candidate.h"
#include "cstring"

Candidate::Candidate(char* id,char* constituency, char* address, int age, int votes)
{
    strcpy(this->constituency, constituency);
    strcpy(this->address, address);
    this->age = age;
    strcpy(this->id,id);
    this->votes=votes;
}
