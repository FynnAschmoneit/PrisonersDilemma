// node.cpp

class Node{
private:
    void copy(const Node& in);
    
public:
    int* Outlinks;
    int* Inlinks;
    int lenOutlinks;
    int lenInlinks;
    bool attractor;
    bool deleted;
    
    Node();
    Node(const Node&);
};

Node::Node(){}

Node::Node(const Node& in){
    copy(in);
}

void Node::copy(const Node& in){
    lenInlinks = in.lenInlinks;
    lenOutlinks = in.lenOutlinks;
    Outlinks = in.Outlinks;
    Inlinks = in.Inlinks;
    attractor = in.attractor;
    deleted = in.deleted;
}


/*
// mti c.copy() einfach aufrufen: a=c.copy();
Node copyNode(Node oldNode){
    Node copyNode;
    
    copyNode.lenInlinks = oldNode.lenInlinks;
    copyNode.lenOutlinks = oldNode.lenOutlinks;
    copyNode.Inlinks = oldNode.Inlinks;
    copyNode.Outlinks = oldNode.Outlinks;
    copyNode.attractor = oldNode.attractor;
    
    return copyNode;
}
*/



