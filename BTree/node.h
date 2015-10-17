#ifndef __BTREE__
#define __BTREE__

#define KEEP_TRACK 1
#if KEEP_TRACK
#define PTRACK cout
#else
#define PTRACK //
#endif



#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/* n~2n segments, kwnum should -1*/
static const int MINIMUM_DEGREE = 2;

class BTreeNode{
public:
    /*need to save father, better implementation is hadle it in tail recursion*/
    BTreeNode *father;
    /*only leaf node split, new root make tree higher*/
    bool isLeaf;
    /*assuming that we save int in the tree, will promote it*/
    vector<int> key;
    /*child, always 1 more than keys*/
    vector<BTreeNode*> child;
    /*only leaf can be inserted directly*/
    int insertNode(int value);
    /*only leaf can call this func*/
    void splitNode();
    /*self print function for debugging*/
    void printNode();
    /*simple constructor*/
    BTreeNode():isLeaf(true) {}

private:
    /*for testify the tree is a B tree*/
    bool ifKeychildMatch();
};

bool BTreeNode::ifKeychildMatch() {
    PTRACK << "ifKeychildMatch\n";
    if(isLeaf) return true;
    return key.size() == child.size() - 1;
}

/*
when reach the leaf, if keys number in [t-1,2t-1), needn't to split
else, split it. move the middle element to father node.
*/
int BTreeNode::insertNode(int value){
    PTRACK << "insertNode()\n";
    int i;
    for(i=0;i<key.size();i++){
        if(key[i]>value) break;
    }
    key.insert(key.begin()+i, value);
    return i;
}
/*
when leaf node is full, we generate a new root,
link the two halves of leaf node.
attention, copy two halves to new node, modify origin leaf
*/
void BTreeNode::splitNode(){
    PTRACK << "splitNode"<<endl;
    int splitIndex = key.size()/2;
    //create two new node
    BTreeNode *left = new BTreeNode();
    BTreeNode *right = new BTreeNode();
    left->isLeaf = true;
    right->isLeaf = true;
    isLeaf = false;
    //copy key, middle element was not copied
    left->key.insert(left->key.end(), key.begin(), key.begin()+splitIndex);
    right->key.insert(right->key.end(), key.begin()+splitIndex+1, key.end());
    //connect them.
    key = {key[splitIndex]};
    child = {left, right};
}

void BTreeNode::printNode(){
    PTRACK << "printNode()\n";
    BTreeNode mark;
    vector<BTreeNode*> toPrint(1,this);
    while(!toPrint.empty()){
        vector<BTreeNode*> nextLevel;
        for(auto x : toPrint){
            cout <<"[";
            if(x == NULL) cout << "NULL";
            else if(x == &mark){
                cout << "==";
            }
            else{
                cout << "("<<x->child.size()<<")";
                for(auto ii : x->key) cout << ii <<",";
                nextLevel.insert(nextLevel.end(), x->child.begin(), x->child.end());
                nextLevel.push_back(&mark);
            }
            cout <<"] ";
        }
        toPrint = nextLevel;
        cout << endl;
    }
}

#endif
