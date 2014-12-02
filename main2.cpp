
// rewards: like in Axelrod, p31.   T>R>P>S     5>3>1>0



//----------------ToDo------------------
// find maxima in commlums instead of in rows in filtermatrix


#include <iostream>
#include <array>
#include <vector>

#define DEBUG

#ifdef DEBUG
#define debug_print(fmt, ...) printf(fmt, __VA_ARGS__)
#else
#define debug_print(fmt, ...) do {} while (0)
#endif

using namespace std;

//const bool debug = true;




//_________________________functions______________________________________

/*
class Node{
public:
    int *OutLinks;
    int *InLinks:
  //  bool attributor;
    
    
};
*/
 
 
int compete(int (*strategyP1)(bool* , bool* , int ),int (*strategyP2)(bool* , bool* , int ), int length){
    // only the score of player one is calculated! strategy of player one must be the first argument!
    bool histP1[length];
    bool histP2[length];
    int scoreP1 = 0;
    
    for (int i = 0; i< length; i++) {
        histP1[i] = strategyP1(histP1,histP2,i);
        histP2[i] = strategyP2(histP2,histP1,i);
        
        if (histP1[i] == 0) {
            if (histP2[i] == 0) {
                scoreP1 += 1;   // both defecting: punishment
            } else {
                scoreP1 += 5;   // temptation to defect
            }
        } else {
            if (histP2[i] == 1) {
                scoreP1 += 3;   // both cooperating: reward
            }
        }
        debug_print("compete:   round: %d   choice player one: %d   choice player two: %d   score player one: %d \n",i, histP1[i], histP2[i], scoreP1);
    }
    return scoreP1;
}

void saveMat(int** mat, int len, char* fileName){
    // nur quadratisch
    
    FILE *pf;
    pf = fopen(fileName,"w");
    
    for (int i = 0; i < len; i++) {
        for (int j = 0; j < len - 1; j++) {
            fprintf(pf, "%d, ", mat[i][j]);
        }
        fprintf(pf, "%d ", mat[i][len - 1]);
        fprintf(pf, "\n");
    }
    fclose(pf);
    debug_print("saveMat:   \"%s\"  has been created \n", fileName);
}

void filterVec(int* vec, int len){
    // Filter für gagnz schlechte strategien??????
    int high = 0;
    for (int i = 0; i < len; i++) {
        if (high < vec[i]) {
            high = vec[i];
        }
    }
    for (int i = 0; i < len; i++) {
        if (vec[i]==high){
            vec[i]=1;
        } else {
            vec[i]=0;
        }
    }
}

void filterMat(int** mat, int len){
    for (int i = 0; i < len; i++) {
        filterVec(mat[i], len);
    }
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




//___________________________________int main____________________________
int main(int argc, const char * argv[])
{
    
    
    srand (time(NULL));         //random machine seed
    
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
    //int (*arStrat[])(bool*,bool*,int) = {S3,S5};      //to test only two strats against another
    
    
    int numberOfRules = 8;
    int numberOfRounds = 100;
    
    int **ResultMatP1;      //score matrix
    ResultMatP1 = new int*[numberOfRules];
    for (int i = 0; i<numberOfRules; i++) {
        ResultMatP1[i] = new int[numberOfRules];
    }

    // filling of the result matrix:
    for (int i = 0; i < numberOfRules; i++) {
        for (int j = 0; j < numberOfRules; j++) {
            ResultMatP1[j][i] = compete(arStrat[i],arStrat[j],numberOfRounds);
        }
    }
    
    
    
    
    saveMat(ResultMatP1,numberOfRules,(char*)"ErgebnisMatrixPlayerOne.csv");
    
    filterMat(ResultMatP1, numberOfRules);
    
    saveMat(ResultMatP1,numberOfRules,(char*)"VerbindungsMatrix.csv");
    

    return 0;
}


















