#include "Voting.h"

#include <fstream>
#include <conio.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

#define CONSTITUENCY_LIST  "Constituency.dat"
#define CANDIDATE_LIST  "Candidates.dat"
#define USERS_LIST  "Users.dat"
#define VOTER_LIST "Voters.dat"

using namespace std;

Voting::Voting()
{

}

void Voting::verify()
{
    verifyLock = true;
}

void Voting::vote()
{
    char name[50], age, voterId, constituancy[20];
    cout<<"Name\t";
    gets(name);
    cout<<"Age:\t";
    cin>>age;
    cout<<"VoterId:\t";
    cin>>voterId;
    cout<<"Constituancy\n";
    cin>>constituancy;
    for(int i=0;i<voterCount;i++)
    {
        if(strcmpi(name,voterList[i]->name)==0 && age==voterList[i]->age && voterId==voterList[i]->voterId && voterList[i]->hasVoted==false && constituancy==voterList[i]->constituancy)
        {
            for(j=0;j<candidateCount;j++)
            {
                if(strcmpi(candidates[i]->constituency,voterList[i]->constituancy)==0)
                {
                    cout<<j+1<<". "<<candidates[j]->id;
                }
            }
            cout<<"Type the name of the candidate to vote for\n";
            gets(name);
            for(j=0;j<candidateCount;j++)
            {
                if(strcmpi(name,candidates[i]->id)==0)
                {
                    ++candidates[i]->votes;
                }
            }
            voterList[i]->hasVoted==true;
        }
    }




}

