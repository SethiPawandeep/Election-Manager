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
    while(f.read((char*)(voter = (Voter*) malloc(sizeof(Voter))),sizeof(Voter)))
    {
        this->voterList = (Voter**) realloc(voterList, (++this->voterCount) * sizeof(Voter*));
        this->voterList[voterCount-1] = voter;
        if(this->voterList[voterCount-1]->hasVoted == true)
        {
            ++haveVoted;
        }
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
    char tmp[25];
    system("cls");
    cout<<"\n\nEnter Details\n";
    cout<<"Name: ";
    cin.getline(id,20);
    cout<<"Address:";
    cin.getline(address, 100);
    cout<<"Age:";
    cin.getline(age,5);
    do{
        std::cout<<"Select Constituancy:\n";
        listConstituancy();
        strset(tmp,0);
        cin.getline(tmp,2);
        consSelection = atoi(tmp);
        if(consSelection < 0 || consSelection > constituancyCount){
            std::cout<<"Invalid Constituancy Selected\n";
            continue;
        }
        break;
    }while(1);
    int votes = 0;
    candidates[candidateCount] = new Candidate(id, this->constituancies[consSelection-1]->cn,address,atoi(age),votes);
    candidateCount++;
    ofstream fout(CANDIDATE_LIST,std::ios::binary|std::ios::app);
    fout.write((char*)candidates[candidateCount-1],sizeof(Candidate));
    fout.close();
    strset(tmp,0);
    strcpy(tmp,"CL_");
    strcat(tmp,this->constituancies[consSelection-1]->cn);
    fout.open(tmp,ios::binary|ios::app);
    fout.write((char*)&candidates[candidateCount-1],sizeof(Candidate));
    fout.close();
}

void ElectionManager::deleteCandidate()
{   int i;
    for(i = 0; i<candidateCount;i++){
        std::cout<<i+1<<". "<<candidates[i]->getId()<<endl;
    }
    cout<<"Select the candidate to be deleted\n";
    int choice;
    char tmp[100];
    cin.getline(tmp,100);
    choice = atoi(tmp);
    i=choice-1;
    free(candidates[i]);
    while(i<candidateCount)
    {
        candidates[i]=candidates[i+1];
        i++;
    }
    --candidateCount;
    candidates = (Candidate**) realloc(candidates, this->candidateCount * sizeof(Candidate*));
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
        cout<<"First time user\n";
        cout << "Press any key to continue and create account\n";
        _getch();
    }

    char pass[20], confirm[20], id[20];

    while(1)
    {
        cout<<"Id:\t\t\t\t\t(max. 20 characters\n\t\t";
        cin.getline(id,20);
        cout<<"Password:\t\t\t\t\t\t(min. 6 char, max 20)\n\t\t";
        cin.getline(pass,20);
        cout<<"Re-enter password\t\t\t\t\n\t\t";
        cin.getline(confirm,20);
        if(strcmp(confirm,pass)!=0)
        {
            cout<<"Re-enter password\n";
            continue;
        }
        break;
    }

    users[getUserCount()] = new User(id, pass, isFirstUser);
    userCount++;
    ofstream fout(USERS_LIST,std::ios::binary|std::ios::app);
    fout.write((char*)users[getUserCount()-1],sizeof(User));
    fout.close();
}

void ElectionManager::deleteConstituancy(){
    char ch[2], tmp[10];
    int i,j;
    cout<<"Select the constituency to be deleted\n";
    for(i=0;i<getConstituancyCount();i++)
        cout<<i+1<<". "<<constituancies[i]->cn<<endl;
    cin.getline(tmp,10);
    int p = atoi(tmp);
    --p;
    cout<<"WARNING! Deleting a constituency will automatically delete the candidates and voters associated with it."<<std::endl;
    cout<<"Do you wish to proceed?(Y/N)"<<std::endl;
    cin.getline(ch,2);

    if(strcmpi(ch,"y")!=0&&strcmpi(ch,"Y")!=0)
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

void ElectionManager::createConstituancy()
{
    int numToAdd = 0;
    char tmp[25];
    cout<<"Enter the number of constituencies\n";
    cin.getline(tmp,2);
    numToAdd = atoi(tmp);
    numToAdd+=constituancyCount;
    char cName[20];

    for(constituancyCount;constituancyCount<numToAdd;constituancyCount++)
    {
        cout<<"Enter name of constituency # "<<constituancyCount+1;
        cin.getline(cName,20);
        Constituency** tempCons = new Constituency*[constituancyCount + 1];
        for(int i = 0; i < constituancyCount; ++i)
        {
            tempCons[i] = constituancies[i];
        }
        if(constituancies != 0) delete[] constituancies;
        constituancies = tempCons;
        constituancies[constituancyCount] = (Constituency*) malloc(sizeof(Constituency));
        strcpy(constituancies[constituancyCount]->cn, cName);
        ofstream fout;
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
    cout<<"Id:\t\t";
    cin.getline(id1,20);
    std::cout<<"Password:\t";
    cin.getline(pass1,20);

    for(int i=0; i<userCount; i++)
    {
        if(users[i]->verifyLogin(id1, pass1)){
            cout << "Access Granted!"<<std::endl;
            loggedIn=users[i];
            return 1;
        }
    }
    cout<<"Access Denied!"<<std::endl;
    return 0;
}

void ElectionManager::listConstituancy()
{
    for(int i=0;i<constituancyCount;i++)
    {
        cout<<i+1<<". "<<this->constituancies[i]->cn<<std::endl;
    }
}

void ElectionManager::showMenu()
{
    int k = 0;
    while(1)
    {
        system("cls");
        if(haveVoted<voterCount || !isLocked)
        {
            cout<<"1. Constituencies\n";
            if(k==1)
            {
                if(!isLocked)
                {
                    cout<<"\ta. Add\n";
                    cout<<"\tb. View\n";
                    cout<<"\tc. Delete\n";
                }
                else
                {
                    cout<<"\ta. View\n";
                }
            }
            cout<<"2. Candidates\n";
            if(k==2)
            {
                if(!isLocked)
                {
                    cout<<"\ta. Add\n";
                    cout<<"\tb. View\n";
                    cout<<"\tc. Delete\n";
                }
                else
                {
                    cout<<"\ta. View\n";

                }
            }
            if(!isLocked)
            {
                cout<<"3. Voter list\n";
                if(k==3)
                {
                    cout<<"\ta. Add\n";
                    cout<<"\tb. View\n";
                    cout<<"\tc. Delete\n";
                    cout<<"\td. Search\n";
                }
                cout<<"4. Lock Election Manager\n";
                cout<<"6. Logout\n";
            }
            if(isLocked)
            {
                cout<<"3. Vote\n";
            }
            cout<<"Enter your choice\n";
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
                if(isLocked)
                {
                    if(k==3)
                    {
                        vote();
                        continue;
                    }
                }
                continue;
            }

            if(ch=='4')
                lockElectionManager();
            if(k==1)
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
                if(k==3)
                {
                    if(ch=='a'||ch=='A') createVoterList();
                    if(ch=='b'||ch=='B') listVoters();
                    if(ch=='c'||ch=='C') deleteVoter();
                    if(ch=='d'||ch=='D') searchVoter();
                }
                if(ch=='6')
                    return;
            }
        }
        else
        {
            cout<<"1. Results\n";
            cout<<"2. Exit\n";
            cout<<"Enter your choice\n";
            char ch = getch();
            if(ch=='1') showResult();
            if(ch=='2') return;
        }
    }
}


void ElectionManager::listCandidates()
{
    for(int i = 0; i<candidateCount;i++)
    {
        cout<<i+1<<". "<<candidates[i]->getId()<<endl;
    }
    int choice;
    char tmp[2];
    while(1)
    {
        cout<<"Enter your choice to view the candidate or enter 0 to return to the main menu.\n";
        cin.getline(tmp,2);
        choice = atoi(tmp);
        if(choice<0||choice>candidateCount)
        {
            cout<<"Invalid selection. Re-enter.\n";
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
    fstream f(VOTER_LIST,ios::app|ios::binary);
    f.close();
    int i;
    cout<<"Select the constituancy to create the voter list for\n";
    for(i=0;i<constituancyCount;i++)
    {
        cout<<i+1<<". "<<constituancies[i]->cn<<endl;
    }
    int choice;
    char temp[25];
    cin.getline(temp,2);
    choice = atoi(temp);
    cout<<"Enter the number of voters for the constituancy\n";
    int numToAdd;
    strset(temp,0);
    cin.getline(temp,5);
    numToAdd = atoi(temp);
    numToAdd+=voterCount;
    strset(temp,0);
    strcpy(temp,"VL_");
    strcat(temp,constituancies[choice-1]->cn);
    for(voterCount;voterCount<numToAdd;voterCount++)
    {
        Voter* VL = (Voter*) malloc(sizeof(Voter));
        cout<<"Enter details of voter #"<<voterCount+1<<endl;
        cout<<"Name:\t";
        cin.getline(VL->name,50);
        cout<<"Age:\t";
        strset(temp,0);
        cin.getline(temp,2);
        VL->age = atoi(temp);
        cout<<"Address:\t";
        cin.getline(VL->address,100);
        strcpy(VL->constituancy,constituancies[choice-1]->cn);
        char s[5];
        strset(s,0);
        Voter V;
        strset(VL->voterId,0);
        f.open(VOTER_LIST,ios::in|ios::binary);
        int VC = 0;
        i = 0;
        while(i<voterCount)
        {
            if(strcmpi(voterList[i]->constituancy,VL->constituancy)==0)
            {
                ++VC;
            }
            ++i;
        }
        VL->voterId[0] = toupper(constituancies[choice-1]->cn[0]);
        itoa(1000+VC,s,10);
        strcat(VL->voterId,s);
        Voter** tempVoter = new Voter*[voterCount+1];
        for(int i=0;i<voterCount;i++)
        {
            tempVoter[i] = voterList[i];
        }
        if(voterList != 0) delete[] voterList;
        voterList = tempVoter;
        voterList[voterCount] = VL;   
    }
    f.open(temp,ios::out|ios::binary);
    i = 0;
    while(i<voterCount)
    {
        if(strcmpi(voterList[i]->constituancy,constituancies[choice-1]->cn)==0)
        {
            f.write((char*)voterList[i],sizeof(Voter));
        }
        ++i;
    }
    f.close();
    f.open(VOTER_LIST,ios::out|ios::binary);
    i = 0;
    while(i<voterCount)
    {
        f.write((char*)voterList[i],sizeof(Voter));
        ++i;
    }
    f.close();
    cout<<"Voter List created.";
}

void ElectionManager::listVoters()
{
    int i;
    ifstream fin;
    char name[50];
    char tmp[25];
    int A[voterCount];
    while(1)
    {
        for(i=0;i<constituancyCount;i++)
        {
            cout<<i+1<<". "<<constituancies[i]->cn<<endl;
        }
        int choice;
        while(1)
        {
            cout<<"Select the constituancy to view the voter list for. Enter 0 to return to the main menu.\n";
            strset(tmp,0);
            cin.getline(tmp,2);
            choice = atoi(tmp);
            if(choice<0||choice>constituancyCount)
                cout<<"Invalid selection.";
            else if(choice==0)
                return;
            else
                break;
        }
        int j = 0;
        system("cls");
        for(i=0;i<voterCount;i++)
        {
            if(strcmpi(voterList[i]->constituancy, constituancies[choice-1]->cn)==0)
            {
                ++j;
                cout<<j<<". "<<voterList[i]->name<<endl;
                A[j-1] = i;
            }
        }
        cout<<"Select the voter to view information";
        cin.getline(tmp,2);
        j = atoi(tmp);
        cout<<"Name:\t"<<voterList[A[j-1]]->name<<endl;
        cout<<"Address:\t"<<voterList[A[j-1]]->address<<endl;
        cout<<"Age:\t"<<voterList[A[j-1]]->age<<endl;
        cout<<"Voter Id:\t"<<voterList[A[j-1]]->voterId<<endl;
        cout<<"Constituancy:\t"<<voterList[A[j-1]]->constituancy;
        _getch();
        cout<<"\nEnter 0 to return to the main menu. Enter -1 to return to previous page.\n";
        while(1)
        {
            strset(tmp,0);
            cin.getline(tmp,2);
            choice = atoi(tmp);
            if(choice<-1||choice>0)
                cout<<"Invalid selection.\n";
            else if(choice==0)
                return;
            else
                break;
        }
    }
}

void ElectionManager::deleteVoter()
{
    char vid[5];
    int i = 0, j;
    cout<<"Enter the Voter Id of the voter to be deleted\n";
    cin.getline(vid,5);
    while(i<voterCount)
    {
        if(strcmpi(vid,voterList[i]->voterId)==0)
        {
            free(voterList[i]);
            for(j = i;j<voterCount;j++)
            {
                voterList[j] = voterList[j+1];
            }
            --voterCount;
            break;
        }
        ++i;
    }
    voterList=(Voter**) realloc(voterList,voterCount*sizeof(Voter));
    ofstream fout(VOTER_LIST,ios::binary|ios::out);
    i = 0;
    while(i<voterCount)
    {
        fout.write((char*)&voterList[i],sizeof(Voter));
        ++i;
    }
    fout.close();
}

void ElectionManager::searchVoter()
{
    cout<<"\nSearch voter by\na. name\t\tb. voter id\n";
    char name[50], ch[2], vid[5];
    cin.get(ch,2);
    if(strcmpi(ch,"a")==0)
    {
        cout<<"Enter name of the voter.\n";
        cin.getline(name,50);
    }
    if(strcmpi(ch,"b")==0)
    {
        cout<<"Enter Voter ID of the voter.\n";
        cin.getline(vid,5);
    }
    int flag = 0;
    for(int i=0;i<voterCount;i++)
    {
        if(strcmpi(voterList[i]->voterId,vid)==0 || strcmpi(voterList[i]->name,name)==0)
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
    cout<<"Enter 0 to return to main menu. Press 1 to search for another voter\n";
    cin.getline(ch,2);
    if(strcmpi(ch,"0")==0)
        return;
    if(strcmpi(ch,"1")==0)
        searchVoter();
}

void ElectionManager::lockElectionManager()
{
    cout<<"Selecting this option means that you have added all the voters, constituancies and candidates.\n";
    cout<<"After this you would not be able to add any more voters, constotuacies or candidates\n";
    cout<<"Please enter your id to confirm.\n";
    char id[20], pass[20];
    cout<<"\n\n\t\tId:\t";
    cin.getline(id,20);
    cout<<"\t\tPassword:\t";
    cin.getline(pass,20);
    for(int i=0;i<getUserCount();i++)
    {
        if(users[i]->verifyLogin(id, pass))
        {
            cout<<"Election Manager Locked. Only voting can be performed now\n";
            ofstream fout("LockElectionManager.lck",ios::binary|ios::out);
            fout.close();
            isLocked = true;
        }
    }
}

void ElectionManager::vote()
{
    char name[50], cons[20],temp[3],voterId[6];
    int age, j, k = 0, candidateId[candidateCount];
    cout<<"Name:\t";
    cin.getline(name,50);
    cout<<"Age:\t";
    cin.getline(temp,3);
    age = atoi(temp);
    cout<<"VoterId:\t";
    cin.getline(voterId,6);
    cout<<"Constituancy:\t";
    cin.getline(cons,20);
    for(int i=0;i<voterCount;i++)
    {
        if(strcmpi(name,voterList[i]->name)==0 && age==voterList[i]->age && strcmpi(voterId,voterList[i]->voterId)==0 && voterList[i]->hasVoted==false && strcmpi(cons,voterList[i]->constituancy)==0)
        {
            for(j=0;j<candidateCount;j++)
            {
                if(strcmpi(candidates[j]->getConstituencyName(),voterList[i]->constituancy)==0)
                {
                    cout<<k+1<<". "<<candidates[j]->getId();
                    candidateId[k] = j;
                    ++k;
                }
            }
            cout<<"Select the candidate to vote for\n";
            strset(temp,0);
            cin.getline(temp,3);
            k = atoi(temp);
            candidates[candidateId[k-1]]->vote();
            cout<<"Voting successful\n";
            voterList[i]->hasVoted=true;
            ++haveVoted;
            break;
        }
    }
}

void ElectionManager::showResult()
{
    int i,j = 0, k;
    int candidateVotes[candidateCount], candidateId[candidateCount];
    for(i = 0;i<constituancyCount;i++)
    {
        k = 0;
        for(j = 0; j<candidateCount; j++)
        {
            if(strcmpi(candidates[j]->getConstituencyName(),constituancies[i]->cn) == 0)
            {
                candidateVotes[k] = candidates[j]->getVotes();
                candidateId[k] = j;
                ++k;
            }
        }
        if(k>0)
        {
            int mostVotes = candidateVotes[0], mostVoted = candidateId[0];
            for(j = 1;j<k;j++)
            {
                if(candidateVotes[j]>mostVotes)
                {
                    mostVotes = candidateVotes[j];
                    mostVoted = candidateId[j];
                }
            }
            cout<<"Winner from constituancy "<<constituancies[i]->cn<<" "<<candidates[mostVoted]->getId()<<" received "<<mostVotes<<" votes\n";
        }
        else
        {
            cout<<"No candidates in the constituency "<<constituancies[i]->cn<<endl;
        }
    }
    getch();
}

ElectionManager::~ElectionManager()
{
    ofstream fout(CANDIDATE_LIST,std::ios::binary|std::ios::out);
    int i = 0;
    while(i<candidateCount)
    {
        fout.write((char*)candidates[i],sizeof(Candidate));
        ++i;
    }
    i = 0;
    fout.close();
    fout.open(CONSTITUENCY_LIST,ios::out|ios::binary);
    while(i<constituancyCount)
    {
        fout.write((char*)constituancies[i],sizeof(Constituency));
        ++i;
    }
    fout.close();
    for(i = 0;i<voterCount;i++)
    {
        free(voterList[i]);
    }
    for(i = 0; i<constituancyCount; i++)
    {
        free(constituancies[i]);
    }
    for(i = 0; i<candidateCount; i++)
    {
        free(candidates[i]);
    }
}
