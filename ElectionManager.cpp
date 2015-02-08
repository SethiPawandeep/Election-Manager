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

void password(char*);

ElectionManager::ElectionManager()
{
    fstream f;
    f.open(CONSTITUENCY_LIST,ios::in|ios::binary);
    Constituency* constituancy;
    constituancyCount = 0;
    while(f.read((char*)(constituancy= (Constituency*) malloc(sizeof(Constituency))), sizeof(Constituency)))
    {
        this->constituancies = (Constituency**) realloc(this->constituancies, ++this->constituancyCount * sizeof(Constituency*));
        this->constituancies[constituancyCount - 1] = constituancy;
    }
    free(constituancy);
    f.close();

    f.open(USERS_LIST,ios::in|ios::binary);
    User* user;
    userCount = 0;
    while(f.read((char*)(user = (User*) malloc(sizeof(User))), sizeof(User)))
    {
        this->users = (User**) realloc(this->users, ++this->userCount * sizeof(User*));
        this->users[userCount - 1] = user;
    }
    free(user);
    f.close();

    f.open(CANDIDATE_LIST,ios::in|ios::binary);
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

void password(char s[20])
{
    int j, i = 1;
    char ch;

    strset(s,0);
    s[0] = getch();
    system("cls");
    cout<<"*";

    while(s[i]!=13)
    {
        ch = getch();
        if(ch == 13)
        {
            ++i;
            break;
        }
        if(ch == '\b')
        {
            i = i - 2;
            system("cls");
            for(j = 0; j < i; j++)
            {
                cout<<"*";
            }
        }
        else
        {
            s[i] = ch;
            cout<<"*";
        }
        ++i;
    }
    s[i]='\0';
}

void ElectionManager::createCandidate()
{
    char id[20], address[100],age[5];
    int consSelection;
    char tmp[25];
    system("cls");
    cout<<"\n\nEnter Details\n";
    cout<<"Name: ";
    cin.getline(id,20);
    cout<<"Address:";
    cin.getline(address, 100);
    do{
        cout<<"Age:";
        cin.getline(age,5);
        if(atoi(age)<25)
        {
            cout<<"Candidate must have age above 25.\n";
            return;
        }
        break;
    }while(1);
    candidates = (Candidate **) realloc(candidates, (candidateCount + 1) * sizeof(Candidate*));
    do
    {
        cout<<"Select Constituancy:\n";
        listConstituancy();
        strset(tmp,0);
        cin.getline(tmp,4);
        consSelection = atoi(tmp);
        if(consSelection < 0 || consSelection > constituancyCount)
        {
            cout<<"Invalid Constituancy Selected\n";
            continue;
        }
        break;
    }while(1);
    int votes = 0;
    candidates[candidateCount] = new Candidate(id, this->constituancies[consSelection-1]->cn,address,atoi(age),votes);
    candidateCount++;
    ofstream fout(CANDIDATE_LIST,ios::binary|ios::app);
    fout.write((char*)candidates[candidateCount-1],sizeof(Candidate));
    fout.close();
}

void ElectionManager::deleteCandidate()
{
    int i, choice;
    while(1)
    {
        for(i = 0; i < candidateCount;i++)
        {
            cout<<i+1<<". "<<candidates[i]->getId()<<endl;
        }
        cout<<"Select the candidate to be deleted\n";
        char tmp[100];
        cin.getline(tmp,100);
        choice = atoi(tmp);
        if(choice < 0 || choice > candidateCount)
        {
            cout<<"Invalid selection.\n";
            continue;
        }
        break;
    }
    i = choice-1;
    free(candidates[i]);
    --candidateCount;
    while(i < candidateCount)
    {
        candidates[i] = candidates[i+1];
        ++i;
    }
    candidates = (Candidate**) realloc(candidates, this->candidateCount * sizeof(Candidate*));
    fstream f;
    f.open(CANDIDATE_LIST,ios::out|ios::binary);
    i = 0;
    while(i < candidateCount)
    {
        f.write((char*)candidates[i],sizeof(Candidate));
        ++i;
    }
    f.close();
}

void ElectionManager::createUser(bool isFirstUser = false)
{
    users = (User **) realloc(users, (getUserCount() + 1) * sizeof(User*));

    if(isFirstUser)
    {
        cout<<"\t\tFirst time user\n";
        cout << "\tPress any key to continue and create account\n";
        getch();
    }

    char pass[20], confirm[20], id[20];
    int k = -1;

    cout<<"Id:\t\t\t\t\t(max. 20 characters\n\t\t";
    cin.getline(id,20);

    while(1)
    {
        cout<<"Password:\t\t\t\t\t(min. 6, max 20 char.)\n";
        do
        {
            ++k;
            if(k > 0)
            {
                cout<<"Incorrect password length\n";
            }
            password(pass);
        }while(strlen(pass) < 6);
        k = -1;
        cout<<"\nRe-enter password:\t\t\t\t\n\t\t";
        password(confirm);
        if(strcmp(confirm,pass)!=0)
        {
            cout<<"Password do not match. Re-enter password\n";
            continue;
        }
        break;
    }

    users[getUserCount()] = new User(id, pass, isFirstUser);
    ++userCount;
    ofstream fout(USERS_LIST,ios::binary|ios::app);
    fout.write((char*)users[getUserCount()-1],sizeof(User));
    fout.close();
}

void ElectionManager::deleteConstituancy()
{
    char ch[2], tmp[10];
    int i, j, choice;
    while(1)
    {
        cout<<"Select the constituency to be deleted\n";
        for(i = 0 ; i < getConstituancyCount() ; i++)
        {
            cout<<i+1<<". "<<constituancies[i]->cn<<endl;
        }
        cin.getline(tmp,10);
        choice = atoi(tmp);
        if(choice<=0||choice>getConstituancyCount())
        {
            cout<<"Invalid Selection\n";
            continue;
        }
        break;
    }
    --choice;
    cout<<"\t\t\tWARNING!\n Deleting a constituency will automatically delete the candidates and voters associated with it."<<endl;
    cout<<"Do you wish to proceed?(Y/N)"<<endl;
    cin.getline(ch,2);
    if(strcmpi(ch,"y")!=0&&strcmpi(ch,"Y")!=0)
    {
        return;
    }
    i = 0;
    while(i < voterCount)
    {
        if(strcmp(voterList[i]->constituancy, constituancies[choice]->cn) == 0)
        {
            free(voterList[i]);
            --voterCount;
            j = i--;
            for(j;j<voterCount;j++)
            {
                voterList[j]=voterList[j+1];
            }
        }
        ++i;
    }
    voterList = (Voter**) realloc(voterList, voterCount * sizeof(Voter));
    ofstream fout(VOTER_LIST,ios::binary|ios::out);
    i = 0;
    while(i < voterCount)
    {
        fout.write((char*)voterList[i],sizeof(Voter));
        ++i;
    }
    fout.close();
    i = 0;
    while(i < candidateCount)
    {
        if(strcmp(candidates[i]->getConstituencyName(),constituancies[choice]->cn)==0)
        {
            free(candidates[i]);
            --candidateCount;
            j = i--;
            for(j;j < candidateCount;j++)
            {
                candidates[j]=candidates[j+1];
            }
        }
        ++i;
    }
    candidates = (Candidate**) realloc(candidates, candidateCount * sizeof(Candidate*));
    fout.open(CANDIDATE_LIST,ios::binary|ios::out);
    i = 0;
    while(i < candidateCount)
    {
        fout.write((char*)candidates[i],sizeof(Candidate));
        ++i;
    }
    fout.close();
    i = 0;
    cout<<"Constituancy with name "<<constituancies[choice]->cn<<" deleted.\n";
    while(i < getConstituancyCount())
    {
        if(i == choice)
        {
            free(constituancies[i]);
            j = i;
            for(j;j<constituancyCount - 1;j++)
            {
                constituancies[j] = constituancies[j+1];
            }
            break;
        }
        ++i;
    }
    --constituancyCount;
    constituancies = (Constituency**) realloc(constituancies,constituancyCount*sizeof(Constituency));
    fout.open(CONSTITUENCY_LIST,ios::binary|ios::out);
    i = 0;
    while(i < getConstituancyCount())
    {
        fout.write((char*)constituancies[i],sizeof(Constituency));
        ++i;
    }
    fout.close();
}

void ElectionManager::createConstituancy()
{
    int numToAdd = 0, flag, i;
    char tmp[25];
    cout<<"Enter the number of constituencies\n";
    cin.getline(tmp,2);
    numToAdd = atoi(tmp);
    numToAdd+=constituancyCount;
    char cName[20];
    for(constituancyCount; constituancyCount < numToAdd; constituancyCount++)
    {
        flag = 1;
        while(flag)
        {
            flag = 0;
            cout<<"Enter name of constituency #"<<constituancyCount+1<<"\t";
            cin.getline(cName,20);
            for(i = 0; i < constituancyCount; ++i)
            {
                if(strcmpi(cName,constituancies[i]->cn) == 0)
                {
                    cout<<"Constituancy with the name "<<cName<<" already exists.\n";
                    flag = 1;
                    break;
                }
            }
        }
        Constituency** tempCons = new Constituency*[constituancyCount + 1];
        for(i = 0; i < constituancyCount; ++i)
        {
            tempCons[i] = constituancies[i];
        }
        if(constituancies != 0) delete[] constituancies;
        constituancies = tempCons;
        constituancies[constituancyCount] = (Constituency*) malloc(sizeof(Constituency));
        strcpy(constituancies[constituancyCount]->cn, cName);
        ofstream fout;
        fout.open(CONSTITUENCY_LIST,ios::binary|ios::app);
        fout.write((char*)constituancies[constituancyCount],sizeof(Constituency));
        fout.close();
    }
}

int ElectionManager::loginPrompt()
{
    system("cls");
    char id[20], pass[20];
    cout<<"Id:\t\t";
    cin.getline(id,20);
    cout<<"Password:\t";
    int i = 0;

    password(pass);

    for(i=0; i<userCount; i++)
    {
        if(users[i]->verifyLogin(id, pass))
        {
            cout << "Access Granted!"<<endl;
            loggedIn=users[i];
            return 1;
        }
    }
    cout<<"Wrong Id/Password combination. Access Denied!"<<endl;
    return 0;
}

void ElectionManager::listConstituancy()
{
    for(int i = 0; i < constituancyCount; i++)
    {
        cout<<i+1<<". "<<this->constituancies[i]->cn<<endl;
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
            if(k == 1)
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
            if(k == 2)
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
                if(k == 3)
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
                cout<<"4. Exit\n";
            }
            cout<<"Enter your choice\n";
            char ch=getch();
            if(ch == '1')
            {
                k = 1;
                continue;
            }
            if(ch == '2')
            {
                k = 2;
                continue;
            }
            if(ch == '3')
            {
                k = 3;
                if(isLocked)
                {
                    if(k == 3)
                    {
                        vote();
                        continue;
                    }
                }
                continue;
            }
            if(ch == '4' && isLocked)
            {
                return;
            }
            if(ch == '4')
                lockElectionManager();
            if(k == 1)
            {
                if(!isLocked)
                {
                    if(ch == 'a'||ch == 'A') createConstituancy();
                    if(ch == 'b'||ch == 'B')
                    {
                        listConstituancy();
                        getch();
                    }
                    if(ch == 'c'||ch == 'C') deleteConstituancy();
                }
                else
                {
                    if(ch == 'a'||ch == 'A')
                    {
                        listConstituancy();
                        getch();
                    }
                }
            }

            if(k == 2)
            {
                if(!isLocked)
                {
                    if(ch == 'a'||ch == 'A') createCandidate();
                    if(ch == 'b'||ch == 'B') listCandidates();
                    if(ch == 'C'||ch == 'c') deleteCandidate();
                }
                else
                {
                    if(ch == 'a'||ch == 'A')listCandidates();
                }
            }
            if(!isLocked)
            {
                if(k == 3)
                {
                    if(ch == 'a'||ch == 'A') createVoterList();
                    if(ch == 'b'||ch == 'B') listVoters();
                    if(ch == 'c'||ch == 'C') deleteVoter();
                    if(ch == 'd'||ch == 'D') searchVoter();
                }
                if(ch == '6')
                    return;
            }
        }
        else
        {
            cout<<"1. Results\n";
            cout<<"2. Exit\n";
            cout<<"Enter your choice\n";
            char ch = getch();
            if(ch == '1') showResult();
            if(ch == '2') return;
        }
    }
}


void ElectionManager::listCandidates()
{
    for(int i = 0; i < candidateCount; i++)
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
        if(choice < 0 || choice > candidateCount)
        {
            cout<<"Invalid selection. Re-enter.\n";
            continue;
        }
        if(choice == 0)
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
    int i;
    cout<<"Select the constituancy to create the voter list for\n";
    for(i = 0; i < constituancyCount; i++)
    {
        cout<<i+1<<". "<<constituancies[i]->cn<<endl;
    }
    int choice, numToAdd;
    char temp[25];
    cin.getline(temp,3);
    choice = atoi(temp);
    cout<<"Enter the number of voters for the constituancy\n";
    strset(temp,0);
    cin.getline(temp,5);
    numToAdd = atoi(temp);
    numToAdd+= voterCount;
    for(voterCount; voterCount < numToAdd; voterCount++)
    {
        Voter* VL = (Voter*) malloc(sizeof(Voter));
        cout<<"Enter details of voter #"<<voterCount+1<<endl;
        cout<<"Name:\t";
        cin.getline(VL->name,50);
        while(1)
        {
            cout<<"Age:\t";
            strset(temp,0);
            cin.getline(temp,3);
            VL->age = atoi(temp);
            if(VL->age < 18)
            {
                cout<<"Age of voter cannot be less than 18\n";
                cout<<"Re-enter age.\n";
                continue;
            }
            break;
        }
        cout<<"Address:\t";
        cin.getline(VL->address,100);
        strcpy(VL->constituancy,constituancies[choice-1]->cn);
        char s[5];
        strset(s,0);
        Voter V;
        strset(VL->voterId,0);
        int VC = 0;
        i = 0;
        while(i < voterCount)
        {
            if(strcmpi(voterList[i]->constituancy,VL->constituancy) == 0)
            {
                ++VC;
            }
            ++i;
        }
        VL->voterId[0] = toupper(constituancies[choice-1]->cn[0]);
        itoa(1000+VC,s,10);
        strcat(VL->voterId,s);
        Voter** tempVoter = new Voter*[voterCount+1];
        for(int i = 0; i < voterCount; i++)
        {
            tempVoter[i] = voterList[i];
        }
        if(voterList != 0) delete[] voterList;
        voterList = tempVoter;
        voterList[voterCount] = VL;   
    }
    fstream f;
    f.open(VOTER_LIST,ios::out|ios::binary);
    i = 0;
    while(i < voterCount)
    {
        f.write((char*)voterList[i],sizeof(Voter));
        ++i;
    }
    f.close();
    cout<<"Voter List created.\n";
}

void ElectionManager::listVoters()
{
    int i, A[voterCount];
    ifstream fin;
    char name[50], tmp[25];
    while(1)
    {
        for(i = 0; i < constituancyCount; i++)
        {
            cout<<i+1<<". "<<constituancies[i]->cn<<endl;
        }
        int choice;
        while(1)
        {
            cout<<"Select the constituancy to view the voter list for. Enter 0 to return to the main menu.\n";
            strset(tmp,0);
            cin.getline(tmp,10);
            choice = atoi(tmp);
            if(choice <= 0 || choice > constituancyCount)
                cout<<"Invalid selection.\n";
            else if(choice == 0)
                return;
            else
                break;
        }
        int j = 0;
        system("cls");
        for(i = 0; i < voterCount; i++)
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
        getch();

        cout<<"\nEnter 0 to return to the main menu. Enter -1 to return to previous page.\n";
        while(1)
        {
            strset(tmp,0);
            cin.getline(tmp,10);
            choice = atoi(tmp);
            if(choice < -1 || choice > 0)
                cout<<"Invalid selection.\n";
            else if(choice == 0)
                return;
            else
                break;
        }
    }
}

void ElectionManager::deleteVoter()
{
    char vid[6];
    int i = 0, j, flag = 0;
    cout<<"Enter the Voter Id of the voter to be deleted\n";
    cin.getline(vid,6);
    while(i < voterCount)
    {
        if(strcmpi(vid,voterList[i]->voterId) == 0)
        {
            flag = 1;
            free(voterList[i]);
            for(j = i; j < voterCount; j++)
            {
                voterList[j] = voterList[j+1];
            }
            --voterCount;
            break;
        }
        ++i;
    }
    if(!flag)
    {
        cout<<"Voter not found\n";
        getch();
        return;
    }
    voterList = (Voter**) realloc(voterList,voterCount*sizeof(Voter));
    ofstream fout(VOTER_LIST,ios::binary|ios::out);
    i = 0;
    while(i < voterCount)
    {
        fout.write((char*)voterList[i],sizeof(Voter));
        ++i;
    }
    fout.close();
}

void ElectionManager::searchVoter()
{
    cout<<"\nSearch voter by\na. name\t\tb. voter id\n";
    char name[50], ch, vid[5];
    ch = getch();
    if(ch == 'a' || ch == 'A')
    {
        cout<<"Enter name of the voter.\n";
        cin.getline(name,50);
    }
    if(ch == 'b' || ch == 'B')
    {
        cout<<"Enter Voter ID of the voter.\n";
        cin.getline(vid,6);
    }
    int flag = 0;
    for(int i = 0; i < voterCount; i++)
    {
        if(strcmpi(voterList[i]->voterId,vid)==0 || strcmpi(voterList[i]->name,name) == 0)
        {
            flag = 1;
            cout<<"Name:\t"<<voterList[i]->name<<endl;
            cout<<"Age:\t"<<voterList[i]->age<<endl;
            cout<<"VoterId:\t"<<voterList[i]->voterId<<endl;
            cout<<"Address:\t"<<voterList[i]->address<<endl;
            cout<<"Constituancy:\t"<<voterList[i]->constituancy<<endl;
            break;
        }
    }
    if(!flag)
    {
        cout<<"Voter not found.\n";
    }
    cout<<"Press 0 to return to main menu.\nPress 1 to search for another voter\n";
    ch = getch();
    if(ch == '0')
        return;
    if(ch == '1')
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
    password(pass);
    for(int i = 0; i < getUserCount(); i++)
    {
        if(users[i]->verifyLogin(id, pass))
        {
            cout<<"Election Manager Locked. Only voting can be performed now\n";
            ofstream fout("LockElectionManager.lck",ios::binary|ios::out);
            fout.close();
            isLocked = true;
        }
        else
        {
            cout<<"Incorrect Id/Password Combination\n";
            getch();
        }
    }
}

void ElectionManager::vote()
{
    char name[50], cons[20], temp[3], voterId[6];
    int age, flag = 0, i, j, k = 0, candidateId[candidateCount];
    cout<<"Name:\t";
    cin.getline(name,50);
    cout<<"Age:\t";
    cin.getline(temp,3);
    age = atoi(temp);
    cout<<"VoterId:\t";
    cin.getline(voterId,6);
    cout<<"Constituancy:\t";
    cin.getline(cons,20);
    for(i = 0; i < voterCount; i++)
    {
        if(strcmpi(name,voterList[i]->name)==0 && age==voterList[i]->age && strcmpi(voterId,voterList[i]->voterId)==0 && voterList[i]->hasVoted==true && strcmpi(cons,voterList[i]->constituancy)==0)
        {
            flag = 0;
            cout<<"Voter has already voted\n";
            getch();
            break;
        }
        if(strcmpi(name,voterList[i]->name)==0 && age==voterList[i]->age && strcmpi(voterId,voterList[i]->voterId)==0 && voterList[i]->hasVoted==false && strcmpi(cons,voterList[i]->constituancy)==0)
        {
            flag = 0;
            for(j = 0; j < candidateCount; j++)
            {
                if(strcmpi(candidates[j]->getConstituencyName(),voterList[i]->constituancy)==0)
                {
                    cout<<k+1<<". "<<candidates[j]->getId()<<endl;
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
        else
        {
            flag = 1;
        }
    }
    if(flag)
    {
        cout<<"Information entered does not match records. Please check and re-enter.\n";
        getch();
    }
    ofstream fout;
    fout.open(VOTER_LIST,ios::out|ios::binary);
    i = 0;
    while(i < voterCount)
    {
        fout.write((char*)voterList[i],sizeof(Voter));
        ++i;
    }
    fout.close();
    fout.open(CANDIDATE_LIST,ios::out|ios::binary);
    i = 0;
    while(i < candidateCount)
    {
        fout.write((char*)candidates[i],sizeof(Candidate));
        ++i;
    }
    fout.close();
}

void ElectionManager::showResult()
{
    int i,j = 0, k;
    int candidateVotes[candidateCount], candidateId[candidateCount];
    for(i = 0;i<constituancyCount;i++)
    {
        k = 0;
        for(j = 0; j < candidateCount; j++)
        {
            if(strcmpi(candidates[j]->getConstituencyName(),constituancies[i]->cn) == 0)
            {
                candidateVotes[k] = candidates[j]->getVotes();
                candidateId[k] = j;
                ++k;
            }
        }
        if(k > 0)
        {
            int mostVotes = candidateVotes[0], mostVoted = candidateId[0];
            for(j = 1; j < k; j++)
            {
                if(candidateVotes[j] > mostVotes)
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
    int i;
    for(i = 0; i < voterCount; i++)
    {
        free(voterList[i]);
    }
    for(i = 0; i < constituancyCount; i++)
    {
        free(constituancies[i]);
    }
    for(i = 0; i < candidateCount; i++)
    {
        free(candidates[i]);
    }
    for(i = 0; i < getUserCount(); i++)
    {
        free(users[i]);
    }
}
