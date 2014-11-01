#ifndef ELECTIONMANAGER_H
#define ELECTIONMANAGER_H

#include "Constituency.h"
#include "User.h"
#include "Candidate.h"

class ElectionManager{
    private:
        int constituancyCount;
        int userCount;
        int candidateCount;
        Constituency cons[20];
        User** users = 0;
        Candidate** candidates = 0;
        User* loggedIn;
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
        void createConstituancy();
        void deleteCandidacy();  //to delete candi.
        void deleteConstituancy();//to del consti.
        void listCandidates();
        void listConstituancy();
};

#endif // ELECTIONMANAGER_H
