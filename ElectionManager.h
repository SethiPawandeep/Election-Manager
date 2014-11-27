#ifndef ELECTIONMANAGER_H
#define ELECTIONMANAGER_H

#include "Constituency.h"
#include "User.h"
#include "Candidate.h"
#include "VoterList.h"
class ElectionManager{
    private:
        int constituancyCount;
        int voterCount;
        int userCount;
        int candidateCount;
        Constituency** constituancies;
        User** users = 0;
        Candidate** candidates = 0;
        User* loggedIn;
        VoterList** voterList;
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
        void editCandidate();
        void createConstituancy();
        void deleteCandidate();  //to delete candi.
        void deleteConstituancy();//to del consti.
        void listCandidates();
        void listConstituancy();
        void createVoterList();
};

#endif // ELECTIONMANAGER_H
