#ifndef CANDIDATE_H
#define CANDIDATE_H

#include "User.h"

class Candidate:public User
{
    private:
        char constituency[20],address[100];//constituency name, name,address of candi.
        int age;	//age of candidate
    public:
        Candidate(char*, char*, char*, char*, int);
        char* getConstituencyName()
        {
            return constituency;
        }
};

#endif // CANDIDATE_H
