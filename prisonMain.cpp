
// rewards: like in Axelrod, p31.   T>R>P>S     5>3>1>0

//graphviz:
//dot testnetwork
//dot -oout.pdf -Tpdf testnetwork
//dot -oout.ps -Tps testnetwork

//dot initialNwSketch && dot -oinitialNw.pdf -Tpdf initialNwSketch && dot interimNwSketch && dot -ointerimNw.pdf -Tpdf interimNwSketch && dot finalNwSketch && dot -ofinalNw.pdf -Tpdf finalNwSketch

// call with debian: g++ -std=gnu++11 prisonMain.cpp

//#define DEBUG

#ifdef DEBUG
#define debug_print(fmt, ...) printf(fmt, __VA_ARGS__)
#else
#define debug_print(fmt, ...) do {} while (0)
#endif


#include <iostream>
#include <array>
#include <vector>
#include <tuple>

//global:
    bool noiseOn = true;
    int chance = 20; // in comparison to 10000


#include "auxiliary.cpp"
#include "strategies.cpp"
#include "node.cpp"
#include "Network.cpp"


using namespace std;


int main(int argc, const char * argv[])
{

    
    int randomSeed = 2;
    //srand (time(NULL));         //random machine seed
    srand (randomSeed);                    // same random number for testing only
    // srand (3);       // GOOD FOR TESTING!!!
    

    // function pointers to strategies:
    int (*S0)(bool*,bool*,int) = TitForTat;
    int (*S1)(bool*,bool*,int) = TatForTit;
    int (*S2)(bool*,bool*,int) = TitForTwoTats;
    int (*S3)(bool*,bool*,int) = TitForThreeTats;
    int (*S4)(bool*,bool*,int) = ExploitTFTT;
    int (*S5)(bool*,bool*,int) = Joss;
    int (*S6)(bool*,bool*,int) = NaivePeaceMaker;
    int (*S7)(bool*,bool*,int) = Friedmann;
    int (*S8)(bool*,bool*,int) = Pavlov;
    int (*S9)(bool*,bool*,int) = Adaptive;
    int (*S10)(bool*,bool*,int) = Envious;
    int (*S11)(bool*,bool*,int) = Cooperate;
    int (*S12)(bool*,bool*,int) = Defect;
    int (*S13)(bool*,bool*,int) = Random;
    
    
    // array of Strategies:
    int (*arStrat[])(bool*,bool*,int) = {S0,S1,S2,S3,S4,S5,S6,S7,S8,S9,S10,S11,S12,S13};
    //int (*arStrat[])(bool*,bool*,int) = {S0,S1,S11,S12};      //to test only two strats against another
    //int (*arStrat[])(bool*,bool*,int) = {S1,S2,S3,S4,S5,S6,S7,S8,S9,S10,S11,S12,S13,S14,S15,S16};

    
    int numberOfRules = 14;
    int numberOfRounds = 100000;
    
    
    
    
    debug_print("------------------------new game with %d rounds, %d strategies and radnom seed %i\n",numberOfRounds,numberOfRules,randomSeed);

    
    
    int **ResultMatP1;      //score matrix
    ResultMatP1 = new int*[numberOfRules];
    for (int i = 0; i<numberOfRules; i++) {
        ResultMatP1[i] = new int[numberOfRules];
    }
    
    debug_print("------------compete%s\n","");
    // filling of the result matrix:
    for (int i = 0; i < numberOfRules; i++) {
        for (int j = 0; j < numberOfRules; j++) {
            ResultMatP1[j][i] = compete(arStrat[i],arStrat[j],numberOfRounds);
        }
    }
    
    int** interimNW = createNullMatrix(numberOfRules);
    int** finalNW = createNullMatrix(numberOfRules);

    
    
    saveMat(ResultMatP1,numberOfRules,(char*)"ErgebnisMatrixPlayerOne.csv");
    
    filterMat(ResultMatP1, numberOfRules);
    
    saveMat(ResultMatP1,numberOfRules,(char*)"VerbindungsMatrix.csv");
    
    createNwSketch(ResultMatP1, numberOfRules, (char*)"initialNwSketch");

    
    Network nw(ResultMatP1,numberOfRules);
    
    
   
    Network nwCopy(ResultMatP1,numberOfRules);
    
    nw.dumpNetwork();
    debug_print("------------reduce network:%s\n","");

    nw.reduceNW1();
    nw.dumpNetwork();
    nw.reduceNW2();
    
    nw.newConnectionMatrix(interimNW);
    
    createNwSketch(interimNW, numberOfRules, (char*)"interimNwSketch");
    
    debug_print("------------expanding network:%s\n","");
    nw.dumpNetwork();

    
    bool alpha = true;
    while (alpha) {
        alpha = nw.expandNW(nwCopy);
        nw.dumpNetwork();
    }
    
    
    nw.newConnectionMatrix(finalNW);

    createNwSketch(finalNW, numberOfRules, (char*)"finalNwSketch");

    
    
    
    /*
    printf("-----------testnetwork:\n");
    int** test= createNullMatrix(3);

    test[0][1] = 1;
    test[1][2] = 1;
    test[2][0] = 1;

    Network nwTEST(test,3);
    Network nwTESTCOPY(test,3);

    nwTEST.dumpNetwork();

    nwTEST.reduceNW1();
    nwTEST.reduceNW2();
    nwTEST.dumpNetwork();

    nwTEST.newConnectionMatrix(interimNW);
    
    bool beta = true;
    int counterExpand = 0;
    while (beta) {
        debug_print("counterExpand: %d\n",counterExpand);
        beta = nwTEST.expandNW(nwTESTCOPY);
        nwTEST.dumpNetwork();
        counterExpand++;
    }
    
    */
    
    
    
       return 0;
}
















