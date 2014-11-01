#ifndef CANDIDATE_H
#define CANDIDATE_H

#include "User.h"

class Candidate
{
    private:
        char constituency[20],address[100],id[50];//constituency name, name,address of candi.
        int age;	//age of candidate
    public:
        Candidate(char*, char*, char*, int);
        inline char* getConstituencyName()
        {
            return constituency;
        }
        inline char* getId()
        {
            return id;
        }
};

#endif // CANDIDATE_H
