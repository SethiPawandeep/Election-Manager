#ifndef USER_H
#define USER_H

class User
{
    private:
        char id[20], pass[20];
        char isAdmin;
    public:
        User(char*, char*, bool);
        char getIsAdmin(){
            return isAdmin;
        }
        char* const getId(){
            return id;
        }

        bool verifyLogin(char* const id, char* const pass);
        void constituency();      //to add consti.
        void edit();    //to edit details of candi.
        void display();//to display details of candi.
        void voterList();
};

#endif // ADMIN_H
