// strategies.cpp


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

int Adaptive(bool* ow, bool* op,int iterationStep){
    //Starts with c,c,c,c,c,c,d,d,d,d,d and then takes choices which have given the best average score re-calculated after every move.
    
    double avC = 0;    // sum score when cooperating
    int counterC = 0;
    double avD = 0;    // sum score when defecting
    if (iterationStep< 6) {
        return 1;
    } else if( iterationStep <11){
        return 0;
    } else {
        
        for (int i = 0; i<iterationStep; i++){
            if (ow[i] == 1) {
                counterC += 1;
                if (op[i] == 1) {  // both cooperating
                    avC = avC + 3;
                }
            } else {
                if (op[i] == 1) {
                    avD = avD + 5;
                } else {
                    avD = avD + 1;
                }
            }
        }
        
        avC = avC/counterC;
        avD = avD/(iterationStep - counterC);
        
        if (avC >= avD) {
            return 1;
        } else {
            return 0;
        }
    }
}

/*
 int Gradual(bool* ow, bool* op,int iterationStep){
 //Gradual - Co-operates until the opponent defects, in such case defects the total number of times the opponent has defected during the game. Followed up by two co-operations
 
 int counterD = 0;
 bool def = false;
 for (int i = 0; i<iterationStep; i++) {
 if (op[i] == 0) {
 counterC += 1;
 }
 }
 
 }
 */


/*
 int Oscaab(bool* ow, bool* op,int iterationStep){
 // ccdd
 //
 bool strat;
 
 
 if (iterationStep < 2){
 return 1;
 }else if (iterationStep < 4) {
 return 0;
 } else if(iterationStep==4) {
 if (op[iterationStep]) {
 strat = 1;
 } else {
 strat = 0;
 }
 }
 }
 */



