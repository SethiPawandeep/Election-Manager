#include "ElectionManager.h"
#include "Voting.h"
#include <stdlib.h>
#include <fstream>

int main()
{
    ElectionManager em;
    Voting v;
    bool emLocked = false;
    ifstream fin("LockElectionManager.dat",ios::in);
    emLocked = fin.read((char*)&v,sizeof(Voting));
    if(!emLocked)
    {
        if(em.getUserCount() < 1)
        em.createUser(true);
        system("cls");
        while(em.loginPrompt() != 1);
        system("cls");
        system("color a");
        em.showMenu();
    }

}
