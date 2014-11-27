#include "ElectionManager.h"
#include "Voting.h"
#include <stdlib.h>
#include <fstream>

int main()
{
    ElectionManager em;
    if(em.getUserCount() < 1)
        em.createUser(true);
    system("cls");
    if(!em.getLocked())
    {
        while(em.loginPrompt() != 1);
    }
    system("cls");
    system("color a");
    em.showMenu();


}
