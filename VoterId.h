#ifndef VOTERID_H
#define VOTERID_H

typedef struct VoterId VoterId;

struct VoterId
{
        char name[50];
        int age;	//vid -> voter id of the voter
        char n[20],vid[7];	//constituency of the voter
};

#endif // VOTERID_H
