#ifndef VOTERID_H
#define VOTERID_H
#include<iostream>
#include<string.h>
using namespace std;

struct VoterList
{
    char name[50], constituancy[20], address[100];
    int age, voterId;

};

//class VoterList
//{
//        char name[50], constituancy[20], address[100];
//        int age, voterId;
//public:
//        inline char* getName()
//        {
//            return name;
//        }
//        inline char* getConstituancy()
//        {
//            return constituancy;
//        }
//        inline int getVoterId()
//        {
//            return voterId;
//        }
//        void enterDetails();
//};

#endif // VOTERID_H
