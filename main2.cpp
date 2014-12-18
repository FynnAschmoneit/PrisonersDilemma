
// rewards: like in Axelrod, p31.   T>R>P>S     5>3>1>0



//----------------ToDo------------------
// find maxima in commlums instead of in rows in filtermatrix
// arraysize für outlinks vergrößern, weil weitere lnks dazukommen....
// 2 regeln: alle weg, die keine selbstabb haben; alle weg, die

//graphviz:
//dot testnetwork
//dot -oout.pdf -Tpdf testnetwork
//dot -oout.ps -Tps testnetwork


#include <iostream>
#include <array>
#include <vector>
#include <tuple>




#define DEBUG

#ifdef DEBUG
#define debug_print(fmt, ...) printf(fmt, __VA_ARGS__)
#else
#define debug_print(fmt, ...) do {} while (0)
#endif

using namespace std;

//_________________________functions______________________________________



class Node{
private:
    
public:
    Node();
    int* Outlinks;
    int* Inlinks;
    int lenOutlinks;
    int lenInlinks;
    bool attractor;
    bool deleted;
};

Node::Node(){}



Node* buildNet(int** mat, int len){
    Node* nodes = new Node[len];
    
    // define index; find lenOutlinks and lenInlinks:
    for (int i = 0; i<len ; i++) {
        nodes[i].attractor = false;
        nodes[i].deleted = false;
        for (int j = 0; j<len; j++) {
            if (mat[i][j] == 1) {
                nodes[i].lenOutlinks += 1;
                nodes[j].lenInlinks += 1;
            }
        }
    }
    
    //filling the arrays Outlinks and Inlinks:
    for (int i = 0; i<len; i++) {
        nodes[i].Outlinks = new int[len];
        int counterOut = 0;
        for (int j = 0; j<len; j++) {
            nodes[i].Outlinks[j] = -1;      // search for -1 to leave the array
            if (mat[i][j] == 1) {
                nodes[i].Outlinks[counterOut] = j;
                counterOut += 1;
            }
        }
    }
    
    for (int i = 0; i<len; i++) {
        nodes[i].Inlinks = new int[len];
        int counterIn = 0;
        for (int j = 0; j<len; j++) {
            nodes[i].Inlinks[j] = -1;
            if (mat[j][i] == 1) {
                nodes[i].Inlinks[counterIn] = j;
                counterIn += 1;
            }
        }
    }
    return nodes;
}

Node copyNode(Node oldNode){
    Node copyNode;
    
    copyNode.lenInlinks = oldNode.lenInlinks;
    copyNode.lenOutlinks = oldNode.lenOutlinks;
    copyNode.Inlinks = oldNode.Inlinks;
    copyNode.Outlinks = oldNode.Outlinks;
    copyNode.attractor = oldNode.attractor;
    
    return copyNode;
}

Node* copyNet(Node* oldNet, int len){
    
    Node* copy = new Node[len];
    
    for (int i = 0; i<len; i++) {
        copy[i] = copyNode(oldNet[i]);
    }
    return copy;
}


bool findElementInArray(int element, int* array, int len){
    for(int i = 0; i<len; i++){
        if(array[i] == element){
            debug_print("findElementInArray:    element %d found in array\n",element);
            return true;
        }
    }
    return false;
}


void dumpArray(int* ar, int len, char* ch ){
    char*  mychar = ch;
    printf("dumpArray:     \"%s\":\n\t\t\t{",mychar);
    for (int i = 0; i<len; i++) {
        printf("%d ",ar[i]);
    }
    printf("}\n");
}


void dumpNetwork(Node* nodes, int len){
    for (int i = 0; i<len; i++) {
        //if (nodes[i].deleted) {continue;}
        
        debug_print("dumpNetwork:   node %d , lenOutlinks: %d, lenInlinks: %d \n                   Outlinks  = { ",i,nodes[i].lenOutlinks,nodes[i].lenInlinks);
        for (int j = 0; j<nodes[i].lenOutlinks ; j++) {
            debug_print("%d ",nodes[i].Outlinks[j]);
        }
        debug_print("} %s \n                   Inlinks = { ","");
        for (int j = 0; j<nodes[i].lenInlinks ; j++) {
            debug_print("%d ",nodes[i].Inlinks[j]);
        }
        debug_print("} %s \n","");
    }
}


void removeLinkFromArray(int link, int* array, int len){
    int counter = 0;
    bool found = false;
    for (int i = 0; i<len; i++) {
        if (array[i] == link) {
            found = true;
        } else {
            array[counter] = array[i];
            counter += 1;
        }
    }
    array[len-1] = -1;
    if (!found) {
        debug_print("removeLinkFromArray:   array does not contain link %d. It can not be deleted from array\n",link);
    }
}


//substitutes the in- and outLinks after a Node is removed. adjusts lenOutlinks, lenInlinks. call after "reduceNetwork"
void substituteLinks(Node* nodes, int len, int indexSubst){
    if (nodes[indexSubst].deleted == false) {
        debug_print("substituteLinks:    ERROR:  substituting of non-deleted node %d!\n", indexSubst);
    } else{
        
        debug_print("substituteLinks:    -------------substituting links of node %d------------\n",indexSubst);
    
        for (int i = 0; i < nodes[indexSubst].lenInlinks; i++) {    // loop over all inlinks
            int indexIn = nodes[indexSubst].Inlinks[i];
            if (indexIn<0) {
                debug_print("substituteLinks:    ERROR: negative indexIn. %s \n","");
            }
        
            for (int j = 0; j < nodes[indexSubst].lenOutlinks; j++) {   // loop over all outlinks
                int indexOut = nodes[indexSubst].Outlinks[j];
        
                if (findElementInArray(indexIn, nodes[indexOut].Inlinks, nodes[indexOut].lenInlinks)) {
                    // indexIn is already connected to indexOut, indpendent of indexSubst.
                
                    debug_print("substituteLinks:    \tlink from node %d to node %d already exists. no links substituted.\n",indexIn,indexOut);
                } else {
                    // link indexIn wird neuer Inlink von nodes[indexOut], length of in-and outlinks of indexOut and indexIn is adjusted
                    
                    nodes[indexIn].Outlinks[nodes[indexIn].lenOutlinks] = indexOut;
                    nodes[indexIn].lenOutlinks += 1;
                    
                    nodes[indexOut].Inlinks[nodes[indexOut].lenInlinks] = indexIn;
                    nodes[indexOut].lenInlinks += 1;
                    
                    debug_print("substituteLinks:    \tnew link from node %d to node %d.\n\t\t\t\tnew number of Outlinks node %d: %d\n\t\t\t\tnew number of Inlinks node %d: %d\n",indexIn,indexOut,indexIn,nodes[indexIn].lenOutlinks, indexOut, nodes[indexOut].lenInlinks);
                    
                    if (indexIn==indexOut) {
                        // new selflink: set delted to false in indexSubst
                        debug_print("substituteLinks:\t\tnew selflink in node %d: deleted set to false\n",indexOut);
                        nodes[indexOut].deleted = false;
                    }
                }
            }
            // "indexSubst" wird als Outlink in indexIn gelöscht und "indexIn" als Inlink von indexSubst gelöscht
        
            removeLinkFromArray(indexSubst, nodes[indexIn].Outlinks, nodes[indexIn].lenOutlinks);
            nodes[indexIn].lenOutlinks -= 1;
            removeLinkFromArray(indexIn, nodes[indexSubst].Inlinks, nodes[indexSubst].lenInlinks);
            nodes[indexSubst].lenInlinks -= 1;
            i -= 1;
            
            debug_print("substituteLinks:    \tOutlink \"%d\" is deleted from Oultinks of node %d and Inlink \"%d\" is deleted from Inlinks of node %d.\n",indexSubst,indexIn,indexIn,indexSubst);
        }

        // deleting "indexSubst" as Inlink in Outlinks and all Outlinks of "indexSubst". indexSubst should not have any more In-or Outlinks.
        
        while(nodes[indexSubst].lenOutlinks > 0) {
            
            int indexOut = nodes[indexSubst].Outlinks[0];
            
            removeLinkFromArray(indexSubst, nodes[indexOut].Inlinks, nodes[indexOut].lenInlinks);
            nodes[indexOut].lenInlinks -= 1;
            
            removeLinkFromArray(indexOut, nodes[indexSubst].Outlinks, nodes[indexSubst].lenOutlinks);
            nodes[indexSubst].lenOutlinks = nodes[indexSubst].lenOutlinks - 1;
            
            debug_print("substituteLinks:    \tInlink \"%d\" is deleted from Inlinks of node %d and Outlink \"%d\" is deleted from Outlinks of node %d:\n",indexSubst,indexOut,indexOut,indexSubst);
         
        }
    }
    dumpNetwork(nodes,len);
}


// checks a network first for nodes without inlinks, then sor selflinks. nodes without inlinks are removed with their outlinks. nodes without selfslink are removed and their links sustituted:
void reduceNetwork1(Node* nodes, int* remainingNodes, int len){
    bool change = true;
    bool hit = false;
    int counter;
    while(change){
        change = false;
        counter = 0;
        
        for (int i = 0; i<len; i++) {
            if (nodes[i].deleted) {continue;}
            remainingNodes[i] = -1;
            hit = false;
            
            if (nodes[i].lenInlinks== 0) {
                debug_print("reduceNetwork1:\tnode %d: no inlink. Set node to deleted\n", i);
                nodes[i].deleted = true;
                change = true;
                
            } else {
                for (int j = 0; j<len; j++){
                    if (nodes[i].Outlinks[j] == i && hit == false ) {
                        debug_print("reduceNetwork1:\tnode %d: selflink \t   (i,j) = %d,%d \n", i, i,j);
                        remainingNodes[counter] = i;
                        counter += 1;
                        hit = true;
                        nodes[i].attractor = true;
                    }
                }
                if (hit == false){      // substitute links as node has inlinks but no selflinks
                    debug_print("reduceNetwork1:\tnode %d: remove and  substitute its links. node set to deleted\n", i);
                    nodes[i].deleted = true;
                    change = true;
                    hit = true;
                }
            }
        }
    }
}


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

    
//Sets highest value in array to one and others to zero
void filterVec(int* vec, int len){
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

    
//  produces the connection matrix from the pay-off matrix. Uses "filterVec" in every row
void filterMat(int** mat, int len){
    for (int i = 0; i < len; i++) {
        filterVec(mat[i], len);
    }
}

void exportNetwork(int** mat, int len, char* fileName){
    
    FILE *pf;
    pf = fopen(fileName,"w");
    
    fprintf(pf, "digraph finite_state_machine {\n\trankdir=LR;\n\tsize=\"8,5\"\n\tnode [shape = circle];\n");
    for (int i = 0; i<len; i++) {
        for (int j = 0; j<len; j++) {
            if (mat[i][j]==1) {
                //debug_print("exportNetwork:     Printing link %d,%d to file \n",i,j);
                fprintf(pf,"\tS%d -> S%d [ label = \"\" ];\n",i,j);
                //fprintf(pf,"%d, %d \n",i,j);
            }
        }
    }
    fprintf(pf,"}\n\n");
    debug_print("exportNetwork:   \"%s\"  has been created \n", fileName);
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
    //srand (1);                    // same random number for testing only
    
    
    // function pointers to strategies:
    int (*S1)(bool*,bool*,int) = TitForTat;
    int (*S2)(bool*,bool*,int) = TatForTit;
    int (*S3)(bool*,bool*,int) = Joss;
    int (*S4)(bool*,bool*,int) = Friedmann;
    int (*S5)(bool*,bool*,int) = TitForTwoTats;
    int (*S6)(bool*,bool*,int) = NaivePeaceMaker;
    int (*S7)(bool*,bool*,int) = Pavlov;
    
    int (*S8)(bool*,bool*,int) = Random;
    
    int (*S9)(bool*,bool*,int) = TitForTat;
    int (*S10)(bool*,bool*,int) = TatForTit;
    int (*S11)(bool*,bool*,int) = Joss;
    int (*S12)(bool*,bool*,int) = Friedmann;
    int (*S13)(bool*,bool*,int) = TitForTwoTats;
    int (*S14)(bool*,bool*,int) = NaivePeaceMaker;
    int (*S15)(bool*,bool*,int) = Pavlov;
    
    int (*S16)(bool*,bool*,int) = Random;
    
    // array of Strategies:
    int (*arStrat[])(bool*,bool*,int) = {S1,S2,S3,S4,S5,S6,S7,S8};
    //int (*arStrat[])(bool*,bool*,int) = {S3,S5};      //to test only two strats against another
    //int (*arStrat[])(bool*,bool*,int) = {S1,S2,S3,S4,S5,S6,S7,S8,S9,S10,S11,S12,S13,S14,S15,S16};

    
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
    
    exportNetwork(ResultMatP1, numberOfRules, (char*)"NetworkSketch");

    
    Node* nodes = new Node[numberOfRules];
    nodes = buildNet(ResultMatP1,numberOfRules);
    
    Node* copyNodes = new Node[numberOfRules];
    copyNodes = copyNet(nodes, numberOfRules);

    
    dumpNetwork(nodes,numberOfRules);
    
    int* remainingNodes = new int[numberOfRules];
    
    reduceNetwork1(nodes, remainingNodes, numberOfRules);
    
    dumpNetwork(nodes,numberOfRules);
    
    dumpArray(remainingNodes,numberOfRules,(char*)  "remaining Nodes");

    
    for (int i = 0; i<numberOfRules; i++) {
        substituteLinks(nodes,numberOfRules,i);
    }
    

    
    
    for (int i = 0; i<10; i++){
        int a = rand() % 10;
        printf("random %d : %d\n",i,a);
    }
    
    
    return 0;
}
















