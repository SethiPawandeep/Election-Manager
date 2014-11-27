#ifndef VOTERID_H
#define VOTERID_H
#include<iostream>
#include<string.h>
using namespace std;

struct Voter
{
    char name[50], constituancy[20], address[100];
    int age, voterId;
    bool hasVoted = false;
};

#endif // VOTERID_H
