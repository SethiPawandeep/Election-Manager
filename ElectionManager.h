#ifndef ELECTIONMANAGER_H
#define ELECTIONMANAGER_H

#include "Constituency.h"
#include "User.h"
#include "Candidate.h"
#include "VoterList.h"
class ElectionManager{
    private:
        bool isLocked;
        int constituancyCount;
        int voterCount;
        int userCount;
        int candidateCount;
        int haveVoted = 0;
        Constituency** constituancies = 0;
        User** users = 0;
        Candidate** candidates = 0;
        User* loggedIn;
        Voter** voterList = 0;
    public:
        ElectionManager();
        int getConstituancyCount(){
            return constituancyCount;
        }
        int getUserCount(){
            return userCount;
        }
        int loginPrompt();
        void showMenu();
        void createUser(bool);
        void createCandidate();
        void deleteCandidate();
        void listCandidates();
        void createConstituancy();
        void deleteConstituancy();
        void listConstituancy();
        void createVoterList();
        void listVoters();
        void deleteVoter();
        void searchVoter();
        void lockElectionManager();
        bool getLocked();
        void vote();
        void showResult();
        ~ElectionManager();
};

#endif // ELECTIONMANAGER_H
