
// rewards: like in Axelrod, p31.   T>R>P>S     5>3>1>0



//----------------ToDo------------------
// 2 regeln: alle weg, die keine selbstabb haben; alle weg, die outlinks haben

//graphviz:
//dot testnetwork
//dot -oout.pdf -Tpdf testnetwork
//dot -oout.ps -Tps testnetwork

#define DEBUG

#ifdef DEBUG
#define debug_print(fmt, ...) printf(fmt, __VA_ARGS__)
#else
#define debug_print(fmt, ...) do {} while (0)
#endif


#include <iostream>
#include <array>
#include <vector>
#include <tuple>

#include "auxiliary.cpp"
#include "strategies.cpp"
#include "node.cpp"
#include "Network.cpp"


using namespace std;


int main(int argc, const char * argv[])
{
    
    
    //srand (time(NULL));         //random machine seed
    srand (2);                    // same random number for testing only
    
    
    // function pointers to strategies:
    int (*S1)(bool*,bool*,int) = TitForTat;
    int (*S2)(bool*,bool*,int) = TatForTit;
    int (*S3)(bool*,bool*,int) = Joss;
    int (*S4)(bool*,bool*,int) = Friedmann;
    int (*S5)(bool*,bool*,int) = TitForTwoTats;
    int (*S6)(bool*,bool*,int) = NaivePeaceMaker;
    int (*S7)(bool*,bool*,int) = Pavlov;
    int (*S8)(bool*,bool*,int) = Adaptive;
    
    int (*S9)(bool*,bool*,int) = Random;
    
    /*
    int (*S9)(bool*,bool*,int) = TitForTat;
    int (*S10)(bool*,bool*,int) = TatForTit;
    int (*S11)(bool*,bool*,int) = Joss;
    int (*S12)(bool*,bool*,int) = Friedmann;
    int (*S13)(bool*,bool*,int) = TitForTwoTats;
    int (*S14)(bool*,bool*,int) = NaivePeaceMaker;
    int (*S15)(bool*,bool*,int) = Pavlov;
    
    int (*S16)(bool*,bool*,int) = Random;
     */
     
    
    // array of Strategies:
    int (*arStrat[])(bool*,bool*,int) = {S1,S2,S3,S4,S5,S6,S7,S8,S9};
    //int (*arStrat[])(bool*,bool*,int) = {S3,S5};      //to test only two strats against another
    //int (*arStrat[])(bool*,bool*,int) = {S1,S2,S3,S4,S5,S6,S7,S8,S9,S10,S11,S12,S13,S14,S15,S16};

    
    int numberOfRules = 9;
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
    
    createNwSketch(ResultMatP1, numberOfRules, (char*)"NetworkSketch");

    
    Network nw(ResultMatP1,numberOfRules);
    
    
    Network nwCopy = nw;
    
    
    nw.dumpNetwork();

    int* remainingNodes = new int[numberOfRules];
    
    nw.reduceNetwork1(remainingNodes);
    
    nw.dumpNetwork();
    
    dumpArray(remainingNodes,numberOfRules,(char*)  "remaining Nodes");

    
    for (int i = 0; i<numberOfRules; i++) {
        nw.substituteLinks(i);
    }
    

    
    
    for (int i = 0; i<10; i++){
        int a = rand() % 10;
        printf("random %d : %d\n",i,a);
    }
    
       return 0;
}
















