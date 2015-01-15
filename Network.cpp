// network.cpp



class Network{
private:
    void copy(const Network& in);
    
public:
    int len;
    Node* nodes;

    Network();
    Network(const Network&);
    Network(int** mat, int n);      // constructor for connection matrix
    void dumpNetwork();
    void substituteLinks(int indexSubst);
    void reduceNetwork1(int* remainingNodes);
    void reduceNW1();
    void reduceNW2();
    void newConnectionMatrix(int** mat);
};

Network::Network(){}


Network::Network(const Network& in){
    copy(in);
}

Network::Network(int** mat, int n){
    len = n;
    
    nodes = new Node[len];
    
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
}

void Network::copy(const Network& in){
    len = in.len;
    nodes = in.nodes;
}


void Network::dumpNetwork(){
    for (int i = 0; i<len; i++) {
        if (!nodes[i].attractor) {continue;}
        
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


void Network::substituteLinks(int indexSubst){
        
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
    dumpNetwork();
}

/*
// deletes all nodes without selflink. other nodes are substituted
void Network::reduceNetwork1(int* remainingNodes){
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
*/

void Network::reduceNW1(){

    for(int i = 0; i<len;i++){
    
        if (nodes[i].lenInlinks== 0) {
            debug_print("reduceNetwork1:\tnode %d: no inlink.\n", i);
        } else if(findElementInArray(i,nodes[i].Inlinks,nodes[i].lenInlinks)){
            debug_print("reduceNetwork1:\tnode %d: selflink \n", i);
            nodes[i].attractor = true;
        } else {
            debug_print("reduceNetwork1:\tnode %d: remove and  substitute its links.\n", i);
            substituteLinks(i);
        }
    }
}


void Network::reduceNW2(){
    for(int i = 0;i<len;i++){
        if (nodes[i].lenOutlinks == 1 && nodes[i].Outlinks[0] == i) {
            debug_print("reduceNW2:     lonely selflink found: %d\n",i);
        } else {
            nodes[i].attractor = false;
        }
    }
}


void Network::newConnectionMatrix(int** mat){
    for (int i = 0; i<len; i++) {
        if (!nodes[i].attractor){continue;}
        for (int j = 0; j<len; j++) {
            if (findElementInArray(j,nodes[i].Outlinks,nodes[i].lenOutlinks)) {
                mat[i][j] = 1;
            }
        }
    }
}























