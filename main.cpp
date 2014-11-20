//
//  main.cpp
//  PrisonersDilemma
//
//  Created by fynn aschmoneit on 18/11/14.
//  Copyright (c) 2014 fynn aschmoneit. All rights reserved.
//

#include <iostream>
#include <array>

using namespace std;



class Entry{
private:
   
    //int len;


public:
    Entry(int length);
    Entry();
    
    bool *history1;          // history of defect(0) or coorperation(1)
    bool *history2;
    int *score;             // counts the score for a given pair of rules
    void SetLength(int length);
    void Player1Cooperate(int index);
    void Player1Defect(int index);
    void Player2Cooperate(int index);
    void Player2Defect(int index);
    bool GetHist1(int index);
    bool GetHist2(int index);
    int UpdateScore(int index);
   // int len;

};



Entry::Entry(){}

Entry::Entry(int length){       //konstruktor
    history1 = new bool[length ];
    history2 = new bool[length];
    score = new int[length];
    for (int i= 0; i<length; i++) {
        score[i] = 0;
    }
}



void Entry::SetLength(int length){
    history1 = new bool[length];
    history2 = new bool[length];
    score = new int[length];
    for (int i= 0; i<length; i++) {
        score[i] = 0;
    }
}


void Entry::Player1Cooperate(int index){
    history1[index] = 1;
}
void Entry::Player1Defect(int index){
    history1[index] = 0;
}
void Entry::Player2Cooperate(int index){
    history2[index] = 1;
}
void Entry::Player2Defect(int index){
    history2[index] = 0;
}

bool Entry::GetHist1(int index){
    bool a = history1[index];
    return a;
}

bool Entry::GetHist2(int index){
    bool a = history2[index];
    return a;
}
/*
int Entry::UpdateScore(int index){
    int a = length;
    return a;
}
*/



//______________________strategies______________________


int TitForTat(bool* ow, bool* op,int iterationStep){
    if (iterationStep==0) {
        return 1;
    }
    else {
        return op[iterationStep-1];
    }
}

int TatForTit(bool* ow, bool* op,int iterationStep){
    if (iterationStep==0) {
        return 0;
    }
    else {
        return op[iterationStep-1];
    }
}

int Random(bool* ow, bool* op,int iterationStep){
    int a = rand() % 2;     // random generator verbessern???
    return a;
}



void play(bool* histP1, bool* histP2 , int iter, int (*strategyP1)(bool* , bool* , int ),int (*strategyP2)(bool* , bool* , int )){
    histP1[iter] = strategyP1(histP1,histP2,iter);
    histP2[iter] = strategyP2(histP2,histP1,iter);
    cout<< "play::    choice player one: " << histP1[iter] << "\n";       //??? strategienamen ausgeben
    cout<< "play::    choice player two: " << histP2[iter] << "\n";
}

//void UpdateScore(){}

int main(int argc, const char * argv[])
{
    srand (time(NULL));
    
    
    // pointers to strategies:
    int (*S1)(bool*,bool*,int) = TitForTat;
    int (*S2)(bool*,bool*,int) = TatForTit;
    int (*S3)(bool*,bool*,int) = Random;
    
    char array1[3];
    
    
    
    Entry test(10);
    for (int i = 0; i<10; i++) {
        //test.history1[i] = TitForTat(test.history2, test.history1, i);
        //test.history2[i] = TitForTat(test.history1, test.history2, i);
        cout<< "iteration step: "<< i << "\n";
        play(test.history2,test.history1,i,S1,S3);
    }

    
    
    
    
    int numberOfRules = 3;
    int numberOfRounds = 20;
   
    
    Entry *history;
    history = new Entry[numberOfRules*numberOfRules];
    
    for (int i = 0; i<numberOfRules*numberOfRules; i++) {
        history[i].SetLength(numberOfRounds);
    }
    
    
    for (int k=0; k<numberOfRounds; k++) {
        //cout << "round no. " << k+1 << "\n";
        int i = 0;
        int j = 0;
        
        for (i=0; i<numberOfRules; i++) {
            for (j=0; j<numberOfRules; j++) {
                // programm öffnen, dass mit i,j die richtigen taktiken öffnet
                // Enrty.updateScore()
                //cout << "Index: " << i <<j <<"\n" ;
            }
        }
    }
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    return 0;
}

