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
    int *history1;          // history of defect(0) or coorperation(1)
    int *history2;
    int *score;             // counts the score for a given pair of rules
    int len;


public:
    Entry(int length);
    Entry();
    void SetLength(int length);
    void Player1Cooperate(int index);
    void Player1Defect(int index);
    void Player2Cooperate(int index);
    void Player2Defect(int index);
    int GetHist1(int index);
    int GetHist2(int index);
    int UpdateScore(int index);
   // int len;

};



Entry::Entry(){}

Entry::Entry(int length){       //konstruktor
    history1 = new int[length ];
    history2 = new int[length];
    score = new int[length];
    for (int i= 0; i<length; i++) {
        score[i] = 0;
    }
}



void Entry::SetLength(int length){
    history1 = new int[length ];
    history2 = new int[length];
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

int Entry::GetHist1(int index){
    int a = history1[index];
    return a;
}

int Entry::GetHist2(int index){
    int a = history2[index];
    return a;
}
/*
int Entry::UpdateScore(int index){
    int a = length;
    return a;
}
*/



//______________________strategies______________________

void TitForTat(Entry a, int player, int iterationStep){
    if (player == 1) {
        if (iterationStep==0) {         //cooperate in step 0
            a.Player1Cooperate(iterationStep);
        }
        else {
            int lastChoiceOpponent = a.GetHist2(iterationStep-1);
            if (lastChoiceOpponent == 1) {
                a.Player1Cooperate(iterationStep);
            }
            else if (lastChoiceOpponent ==0){
                a.Player1Defect(iterationStep);
            }
            else {
            cout << "Error in: TitForTat: bad last choice opponent ";
            }
        }
    }
    else if (player == 2){
        if (iterationStep==0) {         //cooperate in step 0
            a.Player2Cooperate(iterationStep);
        }
        else {
            int lastChoiceOpponent = a.GetHist1(iterationStep-1);
            if (lastChoiceOpponent == 1) {
                a.Player2Cooperate(iterationStep);
            }
            else if (lastChoiceOpponent ==0){
                a.Player2Defect(iterationStep);
            }
            else {
                cout << "Error in: TitForTat: bad last choice opponent ";
            }
        }
    }
}



int main(int argc, const char * argv[])
{
    
//____________TEST______________
    
    
    Entry test(10);
    for (int i = 0; i<10; i++) {
        TitForTat(test, 1, i);
        TitForTat(test, 2, i);
        cout<< "iteration step: "<< i << "\n";
        cout<< "    choice player one: " << test.GetHist1(i)<< "\n";
        cout<< "    choice player two: " << test.GetHist2(i)<< "\n";
       // cout<< "    score: "<< test.UpdateScore(i)<<"\n";

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

