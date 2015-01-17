#ifndef CANDIDATE_H
#define CANDIDATE_H

#include "User.h"

class Candidate
{
    private:
        char constituency[20],address[100],id[50];
        int age, votes = 0;
    public:
        Candidate(char*, char*, char*, int, int);
        inline char* getConstituencyName()
        {
            return constituency;
        }
        inline char* getId()
        {
            return id;
        }
        inline char* getAddress()
        {
            return address;
        }
        inline int getAge()
        {
            return age;
        }
        inline void vote()
        {
            votes++;
        }
        inline int getVotes()
        {
            return votes;
        }
};

#endif // CANDIDATE_H
