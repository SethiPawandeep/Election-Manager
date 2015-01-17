#include "User.h"
#include <cstring>
#include <fstream>

User::User(char* id, char* password, bool isAdmin)
{
    strcpy(this->id, id);
    strcpy(this->pass, password);
    if(isAdmin)
        this->isAdmin = true;
    else
        this->isAdmin = false;
}

bool User::verifyLogin(char* const id, char* const pass)
{
    if(strcmpi(this->id, id)==0 && strcmp(this->pass, pass)==0)
    {
        return 1;
    }
    return 0;
}
