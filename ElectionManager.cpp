#include "ElectionManager.h"
#include "Constituency.h"
#include "Candidate.h"
#include "User.h"
#include "VoterList.h"

#include <cstring>
#include <conio.h>
#include <fstream>
#include <iostream>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

#define CONSTITUENCY_LIST  "Constituency.dat"
#define CANDIDATE_LIST  "Candidates.dat"
#define USERS_LIST  "Users.dat"
#define TEMP  "temp.dat"
#define VOTER_LIST "Voters.dat"

using namespace std;

ElectionManager::ElectionManager(){
    std::fstream f;
    f.open(CONSTITUENCY_LIST,std::ios::in|std::ios::binary);
    int i = 0;
    Constituency* constituancy;
    constituancyCount=0;
    while(f.read((char*)(constituancy= (Constituency*) malloc(sizeof(Constituency))), sizeof(Constituency))){
        this->constituancies = (Constituency**) realloc(this->constituancies, ++this->constituancyCount * sizeof(Constituency*));
        this->constituancies[constituancyCount - 1] = constituancy;
    }
    free(constituancy);
    f.close();

    f.open(USERS_LIST,std::ios::in|std::ios::binary);
    i = 0;
    User* user;
    userCount = 0;
    while(f.read((char*)(user = (User*) malloc(sizeof(User))), sizeof(User))){      //doubt
        this->users = (User**) realloc(this->users, ++this->userCount * sizeof(User*));
        this->users[userCount - 1] = user;
    }
    free(user);
    f.close();

    f.open(CANDIDATE_LIST,std::ios::in|std::ios::binary);
    i = 0;
    Candidate* candidate;
    candidateCount = 0;
    while(f.read((char*)(candidate = (Candidate*) malloc(sizeof(Candidate))), sizeof(Candidate))){
        this->candidates = (Candidate**) realloc(this->candidates, ++this->candidateCount * sizeof(Candidate*));
        this->candidates[candidateCount - 1] = candidate;
    }
    free(candidate);
    f.close();

    f.open(VOTER_LIST,ios::in|ios::binary);
    i = 0;
    VoterList* voter;
    voterCount=0;
    while(f.read((char*)(voter=(VoterList*) malloc(sizeof(VoterList))),sizeof(VoterList)))
    {
        this->voterList = (VoterList**) realloc(this->voterList, ++this->voterCount * sizeof(VoterList*));
        this->voterList[voterCount-1] = voter;
    }
    free(voter);
    f.close();

}

void ElectionManager::createCandidate(){
    candidates = (Candidate **) realloc(candidates, (candidateCount + 1) * sizeof(Candidate*));
    char id[20], address[100],age[5];
    int consSelection,constituancy;
    system("cls");
    cout<<"\n\nEnter Details\n";
    cout<<"Name: ";
    cin>>id;
    std::cout<<"Address:";
    gets(address);//flush input stream
    cin.getline(address, 100);
    std::cout<<"Age:";
    cin>>age;
    do{
        std::cout<<"Select Constituancy:\n";
        listConstituancy();
        cin>>consSelection;
        if(consSelection < 0 || consSelection > constituancyCount){
            std::cout<<"Invalid Constituancy Selected\n";
            continue;
        }
        break;
    }while(1);
    candidates[candidateCount] = new Candidate(id, this->constituancies[consSelection-1]->cn,address,atoi(age));
    candidateCount++;
    std::ofstream fout(CANDIDATE_LIST,std::ios::binary|std::ios::app);
    fout.write((char*)candidates[candidateCount-1],sizeof(Candidate));
    fout.close();
    char tmp[25];
    strset(tmp,0);
    strcpy(tmp,"CL_");
    strcat(tmp,this->constituancies[consSelection-1]);
    fout.open(tmp,ios::binary,ios::app);
    fout.write((char*)&candidates[candidateCount-1],sizeof(Candidate));
    fout.close();
}

void ElectionManager::deleteCandidate()
{   int i;
    Candidate* lastCandidate=candidates[candidateCount];
    for(i = 0; i<candidateCount;i++){
        std::cout<<i+1<<". "<<candidates[i]->getId()<<endl;
    }
    cout<<"Select the candidate to be deleted\n";
    int choice;
    cin>>choice;
    i=choice-1;
    while(i<candidateCount)
    {
        candidates[i]=candidates[i+1];
        i++;
    }
    free(lastCandidate);
    candidateCount--;
    fstream f;
    f.open(CANDIDATE_LIST,ios::out|ios::binary);
    i=0;
    while(i<candidateCount)
    {
        f.write((char*)candidates[i],sizeof(Candidate));
        i++;
    }
    f.close();
}

void ElectionManager::createUser(bool isFirstUser = false)
{
    users = (User **) realloc(users, (getUserCount() + 1) * sizeof(User*));

    if(isFirstUser){
        std::cout<<"First time user\n";
        std::cout << "Press any key to continue and create account\n";
        _getch();
    }

    //    std::fstream f("counter.txt",ios::app|ios::in);
    //    f.seekg(0);
    //    char c;
    //    f.get(c);
    //    c++;
    //    f.seekg(0);
    //    f<<c;

    char pass[20], confirm[20], id[20];

    while(1){
        std::cout<<"Id:\t\t\t\t\t(max. 20 characters\n\t\t";
        std::cin>>id;
        std::cout<<"Password:\t\t\t\t\t\t(min. 6 char, max 20)\n\t\t";
        std::cin>>pass;
        std::cout<<"Re-enter password\t\t\t\t\n";
        std::cin>>confirm;
        if(strcmp(confirm,pass)!=0)
        {
            std::cout<<"Re-enter password\n";
            continue;
        }
        break;
    }

    users[getUserCount()] = new User(id, pass, isFirstUser);
    userCount++;
    std::ofstream fout(USERS_LIST,std::ios::binary|std::ios::app);
    fout.write((char*)users[getUserCount()-1],sizeof(User));
    fout.close();
}

void ElectionManager::deleteConstituancy(){
    char ch;
    int i,j;
    std::cout<<"Select the constituency to be deleted\n";
    for(i=0;i<getConstituancyCount();i++)
       cout<<i+1<<". "<<constituancies[i]->cn<<endl;
    int p;
    cin>>p;
    --p;
    std::cout<<"WARNING! Deleting a constituency will automatically delete the candidates and voters associated with it."<<std::endl;
    std::cout<<"Do you wish to proceed?(Y/N)"<<std::endl;
    ch=_getch();

    if(ch!='y'&&ch!='Y')
    {
        return;
    }
    i=0;
    int votersRemoved=0;
    while(i<votersRemoved)
    {
        if(strcmp(voterList[i]->constituancy, constituancies[p]->cn)==0)
        {
            free(voterList[i]);
            --votersRemoved;
            j=i;
            for(j;j<voterCount;j++)
            {
                voterList[j]=voterList[j+1];
            }
        }
        i++;
    }
    voterCount+=votersRemoved;
    voterList = (VoterList**) realloc(voterList, voterCount * sizeof(VoterList));
    ofstream fout(VOTER_LIST,ios::binary|ios::out);
    i=0;
    char tmp[10];
    strset(tmp,0);
    strcpy(tmp,"VL_");
    strcat(tmp,constituancies[p]->cn);
    while(i<voterCount)
    {
        fout.write((char*)&voterList[i],sizeof(VoterList));
        i++;
    }
    fout.close();
    remove(strcat(tmp,constituancies[p]->cn));
    int candidatesRemoved=0;
    while(i<candidateCount)
    {
        if(strcmp(candidates[i]->getConstituencyName(),constituancies[p]->cn)==0)
        {
            free(candidates[i]);
            candidatesRemoved--;
            j=i;
            for(j;j<candidateCount-1;j++)
            {
                candidates[j]=candidates[j+1];
            }
        }
        i++;
    }
    candidateCount+=candidatesRemoved;
    candidates = (Candidate**) realloc(candidates, candidateCount * sizeof(Candidate*));
    fout.open(CANDIDATE_LIST,std::ios::binary|std::ios::out);
    i=0;
    strset(tmp,0);
    strcat(tmp, "CL_");
    remove(strcat(tmp,constituancies[p]->cn));
    while(i<candidateCount)
    {
        fout.write((char*)candidates[i],sizeof(Candidate));
        i++;
    }
    fout.close();
    i=0;
    while(i<getConstituancyCount())
    {
        if(i>p)
        {
            constituancies[i-1]=constituancies[i];
        }
        i++;
     }
    free(constituancies[constituancyCount]);
    --constituancyCount;
    constituancies=(Constituency**) realloc(constituancies,constituancyCount*sizeof(Constituency));
    fout.open(CONSTITUENCY_LIST,std::ios::binary|std::ios::out);
    i=0;
    while(i<getConstituancyCount())
    {
        fout.write((char*)constituancies[i],sizeof(Constituency));
        i++;
    }
    fout.close();
}

void ElectionManager::createConstituancy(){
    int numToAdd = 0;
    std::cout<<"Enter the number of constituencies\n";
    std::cin>>numToAdd;
    numToAdd+=constituancyCount;
    char cName[20];
    char tmp[25];

    for(constituancyCount;constituancyCount<numToAdd;constituancyCount++)
    {
        std::cout<<"Enter name of constituency #"<<constituancyCount+1;
        std::cin>>cName;
        Constituency** tempCons = new Constituency*[constituancyCount + 1];
        for(int i = 0; i < constituancyCount; ++i){
            tempCons[i] = constituancies[i];
        }
        if(constituancies != 0) delete[] constituancies;
        constituancies = tempCons;
  //      constituancies = (Constituency**)realloc(constituancies,(constituancyCount + 1)*sizeof(Constituency*));
        constituancies[constituancyCount] = (Constituency*) malloc(sizeof(Constituency));
        strcpy(constituancies[constituancyCount]->cn, cName);
        std::ofstream fout;
        fout.open(CONSTITUENCY_LIST,std::ios::binary|std::ios::app);
        fout.write((char*)constituancies[constituancyCount],sizeof(Constituency));
        fout.close();
        strset(tmp, 0);
        strcat(tmp, "CL_");
        fout.open(strcat(tmp,cName),std::ios::binary|std::ios::out);
        fout.close();
        strset(tmp, 0);
        strcat(tmp, "VL_");
        fout.open(strcat(tmp,cName),std::ios::binary|std::ios::out);
        fout.close();
    }
}

int ElectionManager::loginPrompt()
{
    char id1[20],pass1[20];
    //clrscr();
    //gotoxy(35,12);
    std::cout<<"Id:\t\t";
    //gotoxy(30,13);
    //gotoxy(39,12);
    std::cin>>id1;
    std::cout<<"Password:\t";
    //gotoxy(39,13);
    std::cin>>pass1;

    for(int i=0; i<userCount; i++){
        if(users[i]->verifyLogin(id1, pass1)){
            std::cout << "Access Granted!"<<std::endl;
            loggedIn=users[i];
            return 1;
        }
    }
    std::cout<<"Access Denied!"<<std::endl;
    return 0;
}

void ElectionManager::listConstituancy()
{
    for(int i=0;i<constituancyCount;i++){
        std::cout<<i+1<<". "<<this->constituancies[i]->cn<<std::endl;
    }
}

void ElectionManager::showMenu(){
    int k = 0;
    while(1)
    {
        system("cls");
        std::cout<<"1. Constituencies\n";//
        if(k==1)
        {
            std::cout<<"\ta. Add\n";
            std::cout<<"\tb. Delete\n";
            std::cout<<"\tc. View\n";
        }
        std::cout<<"2. Candidates\n";
        if(k==2)
        {
            std::cout<<"\ta. Add\n";
            std::cout<<"\tb. View\n";
            std::cout<<"\tc. Delete\n";
        }
        std::cout<<"3. Voter list\n";
        if(k==3)
        {
            std::cout<<"\ta. Add\n";
            std::cout<<"\tb. Edit\n";
            std::cout<<"\tc. View\n";
        }
        std::cout<<"6. Logout\n";
        std::cout<<"Enter your choice\n";
        char ch=getch();
        if(ch=='1')
        {
            k=1;
            continue;
        }
        if(ch=='2')
        {
            k=2;
            continue;
        }
        if(ch=='3')
        {
            k=3;
            continue;
        }
        if(k==1) //Done
        {
            if(ch=='a'||ch=='A') createConstituancy();//Reduced
            if(ch=='b'||ch=='B') deleteConstituancy();//Reduced
            if(ch=='c'||ch=='C') {
                listConstituancy();
                _getch();
            }
        }
        if(k==2)
        {
            if(ch=='a'||ch=='A') createCandidate();//Done
            if(ch=='b'||ch=='B') {
                listCandidates();//Done
                _getch();
            }
            if(ch=='C'||ch=='c') deleteCandidate();
        }
        if(k==3)
        {
            if(ch=='a'||ch=='A') createVoterList();
        }
        //        if(k==3)
        //        {
        //            if(ch=='a'||ch=='A') Voter_List();
        //            if(ch=='c'||ch=='C')
        //            {
        //                getch();
        //                cout<<"pass1";
        //                ifstream fin("Constituencies.dat",ios::in);
        //                cout<<"Select the constituency to view the voter list\n";
        //                int i=1;
        //                while(fin.read((char*)&b,sizeof(b)))
        //                    cout<<i++<<". "<<b.cn;
        //                fin.close();
        //                i=1;
        //                ch=getch();
        //                char s[50];
        //                strcpy(s,con[i=(int)(ch-'1')].cn);
        //                       strcpy(s,strcat("VL ",s));
        //                       fin.open(s,ios::in|ios::binary);
        //                       clrscr();
        //                       cout<<"scr cleared";
        //                       getch();
        //                       cout<<"S. No.\t"<<"Voter Id.\t"<<"Name\t"<<"Age\n";
        //                       while(fin.read((char*)&v,sizeof(v)))
        //                       cout<<i++<<"\t"<<v.vid<<"\t"<<v.name<<"\t"<<v.age<<endl;
        //                       cout<<"Press 6 to return to previous menu\n";
        //                       cout<<"Press 7 to edit voter list\n";
        //                       ch=getch();
        //                       fin.close();
        //                       if(ch=='6') goto label;
        //            }
        //            }
        if(ch=='6')
            return;
    }
}
//                       int admin::login()
//                {       char id1[20],pass1[20];
//label:
//                    clrscr();
//                    gotoxy(35,12);
//                    cout<<"Id:  ";
//                    gotoxy(30,13);
//                    cout<<"Password\n";
//                    gotoxy(39,12);
//                    cin>>id1;
//                    gotoxy(39,13);
//                    cin>>pass1;
//                    ifstream fin("adminlogin.dat",ios::binary|ios::in);
//                    fin.read((char*)&a1,sizeof(a1));
//                    if((strcmpi(id,id1)==0)&&(strcmp(pass,pass1)==0))
//                    {	cout<<"Access Granted\n";
//                        return 1;
//                    }
//                    else
//                    {	cout<<"Either Id or Password Wrong.\nRe-enter\n";
//                        delay(500);
//                        goto label;
//                    }
//                }

void ElectionManager::listCandidates(){
    for(int i = 0; i<candidateCount;i++){
        std::cout<<i+1<<". "<<candidates[i]->getId()<<endl;
    }
    int choice;

    while(1)
    {  cout<<"Enter your choice to view the candidate or enter 0 to return to the main menu.\n";
             cin>>choice;
        if(choice<0||choice>candidateCount)
        {  cout<<"Invalid selection. Re-enter.\n";
                 continue;
        }
        if(choice==0)
        {
            break;
        }
        cout<<"Name:\t"<<candidates[choice-1]->getId()<<endl;
        cout<<"Age:\t"<<candidates[choice-1]->getAge()<<endl;
        cout<<"Address:\t"<<candidates[choice-1]->getAddress()<<endl;
        cout<<"Constituency:\t"<<candidates[choice-1]->getConstituencyName()<<endl;
    }
}


void ElectionManager::createVoterList()
{
    ofstream fout;
    int i;
    cout<<"Select the constituancy to create the voter list for\n";
    for(i=0;i<constituancyCount;i++)
    {
        cout<<i+1<<constituancies[i]->cn<<endl;
    }
    int choice;
    cin>>choice;
    cout<<"Enter the number of voters for the constituancy\n";
    int numToAdd;
    char temp[25];
    cin>>numToAdd;
    VoterList* VL;
    numToAdd+=voterCount;
    for(voterCount;voterCount<numToAdd;voterCount++)
    {
        cout<<"Name:\t";
        gets(VL->name);//flush input stream
        gets(VL->name);
        cout<<"Age\t";
        cin>>VL->age;
        cout<<"Address:\t";
        gets(VL->address);//flush input stream
        gets(VL->address);
        strcpy(VL->constituancy,constituancies[choice-1]->cn);
        VL->voterId = 1000+voterCount;
        VoterList** tempVoter = new VoterList*[voterCount+1];
        for(int i=0;i<voterCount;i++)
        {
            tempVoter[i]=voterList[i];
        }
        if(voterList != 0) delete[] voterList;
        voterList = tempVoter;
        voterList[voterCount] = (VoterList*) malloc(sizeof(VoterList));
        voterList[voterCount] = VL;
        strset(temp,0);
        strcpy(temp,"VL_");
        strcat(temp,VL->constituancy);
        fout.open(temp,ios::app);
        fout.write((char*)&VL,sizeof(VoterList*));
        fout.close();
        fout.open(VOTER_LIST,ios::app|ios::binary);
        fout.write((char*)&VL,sizeof(VoterList*));
        fout.close();

    }
    cout<<"Voter List created.";
}


