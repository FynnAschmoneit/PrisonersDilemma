// auxiliary


//bool noise(bool in, bool on, int chance){
bool noise(bool in){
	int a  = rand() % 10000; 	
	if(noiseOn && a<= chance){
		return 1 - in;
	} else {
		return in;
	}
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

// speichert eine matrix ab
void createNwSketch(int** mat, int len, char* fileName){
    
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
    debug_print("createNwSketch:   \"%s\"  has been created \n", fileName);
    
}



int** createNullMatrix(int len){
    int **handover;
    handover = new int*[len];
    for (int i = 0; i<len; i++) {
        handover[i] = new int[len];
    }
    for (int i = 0; i<len; i++) {
        for (int j = 0; j<len; j++) {
            handover[i][j] = 0;
        }
    }
    
    return handover;
}
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    

