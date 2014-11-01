#include "ElectionManager.h"
#include <stdlib.h>

int main()
{
    ElectionManager em;
    if(em.getUserCount() < 1)
        em.createUser(true);
    system("cls");
    while(em.loginPrompt() != 1);
    system("cls");
    system("color a");
    em.showMenu();
}
