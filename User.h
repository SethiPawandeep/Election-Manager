#ifndef USER_H
#define USER_H

class User
{
    private:
        char id[20], pass[20];
        char isAdmin;
    public:
        User(char*, char*, bool);
        char getIsAdmin()
        {
            return isAdmin;
        }
        char* const getId()
        {
            return id;
        }

        bool verifyLogin(char* const id, char* const pass);
};

#endif // ADMIN_H
