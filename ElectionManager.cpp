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
#

#define CONSTITUENCY_LIST  "Constituency.dat"
#define CANDIDATE_LIST  "Candidates.dat"
#define USERS_LIST  "Users.dat"
#define VOTER_LIST "Voters.dat"

using namespace std;

ElectionManager::ElectionManager()
{
    std::fstream f;
    f.open(CONSTITUENCY_LIST,std::ios::in|std::ios::binary);
    Constituency* constituancy;
    constituancyCount=0;
    while(f.read((char*)(constituancy= (Constituency*) malloc(sizeof(Constituency))), sizeof(Constituency)))
    {
        this->constituancies = (Constituency**) realloc(this->constituancies, ++this->constituancyCount * sizeof(Constituency*));
        this->constituancies[constituancyCount - 1] = constituancy;
    }
    free(constituancy);
    f.close();

    f.open(USERS_LIST,std::ios::in|std::ios::binary);
    User* user;
    userCount = 0;
    while(f.read((char*)(user = (User*) malloc(sizeof(User))), sizeof(User)))
    {
        this->users = (User**) realloc(this->users, ++this->userCount * sizeof(User*));
        this->users[userCount - 1] = user;
    }
    free(user);
    f.close();

    f.open(CANDIDATE_LIST,std::ios::in|std::ios::binary);
    Candidate* candidate;
    candidateCount = 0;
    while(f.read((char*)(candidate = (Candidate*) malloc(sizeof(Candidate))), sizeof(Candidate))){
        this->candidates = (Candidate**) realloc(this->candidates, ++this->candidateCount * sizeof(Candidate*));
        this->candidates[candidateCount - 1] = candidate;
    }
    free(candidate);
    f.close();

    f.open(VOTER_LIST,ios::in|ios::binary);
    Voter* voter;
    voterCount=0;
    while(f.read((char*)(voter=(Voter*) malloc(sizeof(Voter))),sizeof(Voter)))
    {
        this->voterList = (Voter**) realloc(this->voterList, ++this->voterCount * sizeof(Voter*));
        this->voterList[voterCount-1] = voter;
    }
    free(voter);
    f.close();

    f.open("LockElectionManager.lck",ios::in);
    if(f.is_open())
    {
        isLocked = true;
    }
    else
    {
        isLocked = false;
    }

}

bool ElectionManager::getLocked()
{
    return this->isLocked;
}

void ElectionManager::createCandidate(){
    candidates = (Candidate **) realloc(candidates, (candidateCount + 1) * sizeof(Candidate*));
    char id[20], address[100],age[5];
    int consSelection;
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
    int votes = 0;
    candidates[candidateCount] = new Candidate(id, this->constituancies[consSelection-1]->cn,address,atoi(age),votes);
    candidateCount++;
    std::ofstream fout(CANDIDATE_LIST,std::ios::binary|std::ios::app);
    fout.write((char*)candidates[candidateCount-1],sizeof(Candidate));
    fout.close();
    char tmp[25];
    strset(tmp,0);
    strcpy(tmp,"CL_");
    strcat(tmp,this->constituancies[consSelection-1]->cn);
    fout.open(tmp,ios::binary|ios::app);
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
    voterList = (Voter**) realloc(voterList, voterCount * sizeof(Voter));
    ofstream fout(VOTER_LIST,ios::binary|ios::out);
    i=0;
    char tmp[10];
    strset(tmp,0);
    strcpy(tmp,"VL_");
    strcat(tmp,constituancies[p]->cn);
    while(i<voterCount)
    {
        fout.write((char*)&voterList[i],sizeof(Voter));
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
        std::cout<<"1. Constituencies\n";
        if(k==1)
        {
            if(!isLocked)
            {
                std::cout<<"\ta. Add\n";
                std::cout<<"\tb. View\n";
                std::cout<<"\tc. Delete\n";
            }
            else
            {
                std::cout<<"\ta. View\n";
            }
        }
        std::cout<<"2. Candidates\n";
        if(k==2)
        {
            if(!isLocked)
            {
                std::cout<<"\ta. Add\n";
                std::cout<<"\tb. View\n";
                std::cout<<"\tc. Delete\n";
            }
            else
            {
                std::cout<<"\ta. View\n";

            }
        }
        if(!isLocked)
        {
            std::cout<<"3. Voter list\n";
            if(k==3)
            {
                std::cout<<"\ta. Add\n";
                std::cout<<"\tb. View\n";
                std::cout<<"\tc. Delete\n";
                cout<<"\td. Search\n";
            }
            cout<<"4. Lock Election Manager";
            std::cout<<"6. Logout\n";
        }
        if(isLocked)
        {
            cout<<"3. Vote\n";
        }
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
        if(ch=='4')
            lockElectionManager();
        if(k==1) //Done
        {
            if(!isLocked)
            {
                if(ch=='a'||ch=='A') createConstituancy();
                if(ch=='b'||ch=='B')
                {
                    listConstituancy();
                    _getch();
                }
                if(ch=='c'||ch=='C') deleteConstituancy();

            }
            else
            {
                if(ch=='a'||ch=='A')
                {
                    listConstituancy();
                    _getch();

                }
            }
        }

        if(k==2)
        {
            if(!isLocked)
            {
                if(ch=='a'||ch=='A') createCandidate();
                if(ch=='b'||ch=='B') {
                    listCandidates();
                    _getch();
                }
                if(ch=='C'||ch=='c') deleteCandidate();
            }
            else
            {
                if(ch=='a'||ch=='A')
                {
                    listCandidates();
                    _getch();
                }

            }
        }
            if(!isLocked)
            {
                if(k==3)//Bugs
                {
                    if(ch=='a'||ch=='A') createVoterList();//Error: The inferior stopped working because it received a signal from the Operating System, no voter detail written into the file or in the voterList.
                    if(ch=='b'||ch=='B') listVoters();//Does not work due to the above reason
                    if(ch=='c'||ch=='C') deleteVoter();//Does not work due to the above reason
                    if(ch=='d'||ch=='D') searchVoter();//Does not work due to the above reason
                }
                if(ch=='6')
                    return;
            }
            if(isLocked)
            {
                if(ch=='3')
                {
                    vote();
                }
            }
        }
    }


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
    numToAdd+=voterCount;
    for(voterCount;voterCount<numToAdd;voterCount++)
    {
        Voter* VL = (Voter*) malloc(sizeof(Voter));
        cout<<"Enter details of voter #"<<voterCount<<endl;
        cout<<"Name:\t";
        cin.ignore();
        cin.getline(VL->name,50);
        cout<<"Age\t";
        cin>>VL->age;
        cout<<"Address:\t";
        cin.ignore();
        cin.getline(VL->address,100);
        strcpy(VL->constituancy,constituancies[choice-1]->cn);
        VL->voterId = 1000+voterCount;
        Voter** tempVoter = new Voter*[voterCount+1];
        for(int i=0;i<voterCount;i++)
        {
            tempVoter[i]=voterList[i];
        }
        if(voterList != 0) delete[] voterList;
        voterList = tempVoter;
        voterList[voterCount] = VL;
        strcpy(temp,"VL_");
        strcat(temp,VL->constituancy);
        fout.open(temp,ios::app);
        fout.write((char*)&VL,sizeof(Voter*));
        fout.close();
        fout.open(VOTER_LIST,ios::app|ios::binary);
        fout.write((char*)&VL,sizeof(Voter*));
        fout.close();
    }
    cout<<"Voter List created.";
}

void ElectionManager::listVoters()
{
    int i;
    char name[50];
    cout<<"Select the constituancy to view the voter list for. Enter 0 to return to the main menu.\n";
    int voterSelection=-1;
    while(voterSelection==-1||voterSelection!=0)
    {
        system("cls");
        for(i=0;i<constituancyCount;i++)
        {
            cout<<i+1<<". "<<constituancies[i]->cn<<endl;
        }
        int choice;
        while(1)
        {
            cin>>choice;
            if(choice<0||choice>constituancyCount)
                cout<<"Invalid selection.";
            else if(choice==0)
                return;
            else
                break;
        }
        int j = 0;
        for(i=0;i<voterCount;i++)
        {
            if(strcmpi(voterList[i]->constituancy, constituancies[choice-1]->cn)==0)
            {
                j++;
                cout<<j+1<<". "<<voterList[i]<<endl;
            }
        }
        cout<<"Enter the name of a voter to view information. Enter 0 to return to the main menu. Enter -1 to return to previous page.\n";
        gets(name);
        if(voterSelection!=0||voterSelection!=-1)
        {
            for(i=0;i<voterCount;i++)
            {
                if(strcmpi(voterList[i]->name, name)==0 && strcmpi(voterList[i]->constituancy, constituancies[choice-1]->cn)==0)
                {
                    cout<<"Name:\t"<<voterList[i]->name<<endl;
                    cout<<"Address:\t"<<voterList[i]->address<<endl;
                    cout<<"Age:\t"<<voterList[i]->age;
                    cout<<"Voter Id:\t"<<voterList[i]->voterId<<endl;
                    cout<<"Constituancy:\t"<<voterList[i]->constituancy;
                    break;
                }

            }
        }
        _getch();
    }
}

void ElectionManager::deleteVoter()
{
    int vid;
    cout<<"Enter the name or voter Id of the voter to be deleted\n";
    cin>>vid;
    int i = 0;
    while(i<voterCount)
    {
        if(i>vid-1000)  //since voters are already sorted in order of their voter Ids.
        {
            voterList[i-1]=voterList[i];
        }
        i++;
    }
    free(voterList[voterCount]);
    --voterCount;
    voterList=(Voter**) realloc(voterList,voterCount*sizeof(Voter));
    ofstream fout(VOTER_LIST,ios::binary|ios::out);
    i=0;
    while(i<voterCount)
    {
        fout.write((char*)&voterList[i],sizeof(Voter));
        i++;
    }
    fout.close();
}

void ElectionManager::searchVoter()
{
    cout<<"Search voter by\na. name\t\tb. voter id\n";
    char name[50], ch;
    strset(name,0);
    ch=_getch();
    int vid = 0;
    if(ch=='a')
    {
        cout<<"Enter name of the voter.\n";
        gets(name);
    }
    if(ch=='b')
    {
        cout<<"Enter Voter ID of the voter.\n";
        cin>>vid;
    }
    int flag = 0;
    for(int i=0;i<voterCount;i++)
    {
        if(voterList[i]->voterId==vid || strcmpi(voterList[i]->name,name)==0)
        {
            flag=1;
            cout<<"Name:\t"<<voterList[i]->name;
            cout<<"Age:\t"<<voterList[i]->age;
            cout<<"VoterId:\t"<<voterList[i]->voterId;
            cout<<"Address:\t"<<voterList[i]->address;
            cout<<"Constituancy:\t"<<voterList[i]->constituancy;
            break;
        }
    }
    if(flag==0)
    {
        cout<<"Voter not found.\n";
    }
    cout<<"Press 0 to return to main menu. Press 1 to search for another voter\n";
    ch=_getch();
    if(ch=='0')
        return;
    if(ch=='1')
        searchVoter();
}

void ElectionManager::lockElectionManager()
{
    cout<<"Selecting this option means that you have added all the voters, constituancies and candidates.\n";
    cout<<"After this you would not be able to add any more voters, constotuacies or candidates\n";
    cout<<"Please enter your id to confirm.\n";
    char id[20], pass[20];
    cout<<"\n\n\t\tId:\t";
    cin>>id;
    cout<<"\t\tpass:\t";
    cin>>pass;
    for(int i=0;i<getUserCount();i++)
        if(users[i]->verifyLogin(id, pass))
        {
            cout<<"Election Manager Locked. Only voting can be performed now\n";
        }
    ofstream fout("LockElectionManager.lck",ios::binary|ios::out);
    fout.close();
    isLocked = true;
}

void ElectionManager::vote()
{
    char name[50], age, voterId, constituancy[20];
    cout<<"Name\t";
    gets(name);
    cout<<"Age:\t";
    cin>>age;
    cout<<"VoterId:\t";
    cin>>voterId;
    cout<<"Constituancy\n";
    cin>>constituancy;
    char* candidateName[candidateCount];
    int j, k = 0;
    for(int i=0;i<voterCount;i++)
    {
        if(strcmpi(name,voterList[i]->name)==0 && age==voterList[i]->age && voterId==voterList[i]->voterId && voterList[i]->hasVoted==false && constituancy==voterList[i]->constituancy)
        {
            for(j=0;j<candidateCount;j++)
            {
                if(strcmpi(candidates[j]->getConstituencyName(),voterList[i]->constituancy)==0)
                {
                    cout<<j+1<<". "<<candidates[j]->getId();
                    candidateName[k]==candidates[j]->getId();
                    ++k;
                }
            }
            cout<<"Select the candidate to vote for\n";
            cin>>k;
            for(j=0;j<candidateCount;j++)
            {
                if(strcmpi(candidateName[k-1],candidates[i]->getId())==0)
                {
                    candidates[i]->vote();
                    cout<<"Voting successful\n";
                    break;
                }
            }
            voterList[i]->hasVoted==true;
        }
    }
}
