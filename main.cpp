//
//  main.cpp
//  PrisonersDilemma
//
//  Created by fynn aschmoneit on 18/11/14.
//  Copyright (c) 2014 fynn aschmoneit. All rights reserved.
//


// rewards: like in Axelrod, p31.   T>R>P>S     5>3>1>0




//----------------ToDo------------------
// matrix representation


#include <iostream>
#include <array>

using namespace std;



class Entry{
private:
   

public:
    Entry(int length);
    Entry();
    
    bool *history1;          // history of defect(0) or coorperation(1)
    bool *history2;
    int *score1;             // counts the score for a given pair of rules player one
    int *score2;             // counts the score for a given pair of rules player two
    int *scoreOverall;
    void SetLength(int length);
    int UpdateScore(int index);     // updates the two individual scores and returns the overall score

};


Entry::Entry(){}

Entry::Entry(int length){       //konstruktor
    history1 = new bool[length];
    history2 = new bool[length];
    score1 = new int[length];
    score2 = new int[length];
    scoreOverall = new int[length];
    for (int i= 0; i<length; i++) {
        score1[i] = 0;
        score2[i] = 0;
        scoreOverall[i] = 0;
    }
}

void Entry::SetLength(int length){
    history1 = new bool[length];
    history2 = new bool[length];
    score1 = new int[length];
    score2 = new int[length];
    scoreOverall = new int[length];
    for (int i= 0; i<length; i++) {
        score1[i] = 0;
        score2[i] = 0;
        scoreOverall[i] = 0;

    }
}

int Entry::UpdateScore(int index){         // index = iteration step
    
    if (history1[index]==1) {
        if (history2[index] == 1) {
            // both cooperating: 3 points for both players
            score1[index] = 3;
            score2[index] = 3;
        } else {
            // P1 cooperates; P2 defects: P1 gets 0; P2 gets 5
            score2[index] = 5;
        }
    } else {
        if (history2[index]== 1 ) {
            // P1 defects, P2 cooperates: P1 gets 5; P2 gets 0
            score1[index] = 5;
        } else {
            // both defecting: both get 1
            score1[index] = 1;
            score2[index] = 1;
        }
    }
    
    if (index == 0) {
        scoreOverall[index] = score1[index] + score2[index];
    } else {
    scoreOverall[index] = scoreOverall[index -1 ] + score1[index] + score2[index];
    }
        
    cout << "UpdateScore::      score player 1: "<<score1[index]<< "   score player 2: "<< score2[index]<< "    overall score: " << scoreOverall[index] << "\n" ;
    return scoreOverall[index];
}


 



//______________________strategies______________________
// http://www.iterated-prisoners-dilemma.net/prisoners-dilemma-strategies.shtml#
//http://sciencehouse.wordpress.com/2012/09/04/a-new-strategy-for-the-iterated-prisoners-dilemma-game/

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

int Joss(bool* ow, bool* op,int iterationStep){
    // COOPERATE at first round
    //if other player defected before: DEFECT
    //if other player cooperated before: Defect with p = 10% ; COOPERATE with p=90%
    if (iterationStep==0) {
        return 1;
    }
    else {
        if (op[iterationStep -1] == 0) {
            return 0;
        } else {
            int a = rand() % 10;
            if (a == 0) {
                return 0;
            } else {
                return 1;
            }
        }
    }
}

int Friedmann(bool* ow, bool* op,int iterationStep){
    // COOPERATE first round
    // totally unforgiving after first defect
    if (iterationStep==0) {
        return 1;
    } else {
        for (int i = 0; i<iterationStep; i++) {
            if (op[i]==0) {
                //cout << "Friedmann::    DEFECT because of earlier defect" << endl;
                return 0;
            }
        }
        //cout << "Friedmann::    COOPERATE because no earlier defect" << endl;
        return 1;
    }
}

int TitForTwoTats(bool* ow, bool* op,int iterationStep){
    // COOPERATE first round
    // DEFECT only, if oponent has defected twice in a row before
    if (iterationStep<2) {
        return 1;
    } else{
        if (op[iterationStep -1 ] == 0 && op[iterationStep -2] == 0) {
            return 0;
        } else {
            return 1;
        }
    }
}

int NaivePeaceMaker(bool* ow, bool* op,int iterationStep){
    // COOPERATE at first round
    // COOPERATE when op has cooperated before
    // COOPERATE with p=10%, when op has defected
    // DEFECT with p=90%, when op has defected
    if (iterationStep==0) {
        return 1;
    }
    else {
        if (op[iterationStep -1] == 1) {
            return 1;
        } else {
            int a = rand() % 10;
            if (a == 0) {
                return 1;
            } else {
                return 0;
            }
        }
    }
}

int Pavlov(bool* ow, bool* op,int iterationStep){
    // COOPERATE at first round
    // If 5 or 3 points scored in the last round then repeat last choice.
    if (iterationStep==0) {
        return 1;
    } else {
        if (op[iterationStep-1]==1) {
            return ow[iterationStep - 1];
        } else {
            return 1 - ow[iterationStep-1];
        }
    }
}

/*
int Pavlov(bool* ow, bool* op,int iterationStep){
    //Starts with c,c,c,c,c,c,d,d,d,d,d and then takes choices which have given the best average score re-calculated after every move.
    if (iterationStep< 6) {
        return 1;
    } else if( iterationStep <11){
        return 0;
    } else {
    
    }
}
*/

void play(bool* histP1, bool* histP2 , int iter, int (*strategyP1)(bool* , bool* , int ),int (*strategyP2)(bool* , bool* , int )){
    // (history player one, history player two, iteration step, strategy player one, strategyg player two)
    histP1[iter] = strategyP1(histP1,histP2,iter);
    histP2[iter] = strategyP2(histP2,histP1,iter);
    cout<< "play::    choice player one: " << histP1[iter] << endl;       //??? strategienamen ausgeben
    cout<< "play::    choice player two: " << histP2[iter] << endl;
}


int main(int argc, const char * argv[])
{
    srand (time(NULL));
    
    
    // function pointers to strategies:
    int (*S1)(bool*,bool*,int) = TitForTat;
    int (*S2)(bool*,bool*,int) = TatForTit;
    int (*S3)(bool*,bool*,int) = Joss;
    int (*S4)(bool*,bool*,int) = Friedmann;
    int (*S5)(bool*,bool*,int) = TitForTwoTats;
    int (*S6)(bool*,bool*,int) = NaivePeaceMaker;
    int (*S7)(bool*,bool*,int) = Pavlov;

    int (*S8)(bool*,bool*,int) = Random;


    // array of Strategies:
    int (*arStrat[])(bool*,bool*,int) = {S1,S2,S3,S4,S5,S6,S7,S8};
    
    //int (*arStrat[])(bool*,bool*,int) = {S3,S5};
    
    

    
    int numberOfRules = 8;
    int numberOfRounds = 20;
   
    cout << "\n \n \n \n \n \n--------------------------new game----------------------- \n";
    cout << "number of strategies: " << numberOfRules << endl << "number of rounds: " << numberOfRounds <<"\n \n \n";
    
    Entry *history;
    history = new Entry[numberOfRules*numberOfRules];       //history is an array of Enrty elemenst
    
    
    for (int i = 0; i<numberOfRules*numberOfRules; i++) {
        history[i].SetLength(numberOfRounds);
    }
    
    
    for (int k=0; k<numberOfRounds; k++) {
        cout << "iteration loop::       ------------round no. " << k+1 << "-------------\n";
        int i = 0;
        int j = 0;
        int counter = 0;        // adress of correct element of array "history"
        
        for (i=0; i<numberOfRules; i++) {
            for (j=0; j<numberOfRules; j++) {
                cout << "iteration loop::   matrix index:" << i << j << "\n";
                play(history[counter].history1,history[counter].history2,k,arStrat[i],arStrat[j]);
                int a = history[counter].UpdateScore(k);
                cout << " overall score: " << a << "\n";
                // programm öffnen, dass mit i,j die richtigen taktiken öffnet
                
                
                // Enrty.updateScore()
                //cout << "Index: " << i <<j <<"\n" ;
                counter += 1;
            }
        }
    }
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    return 0;
}

