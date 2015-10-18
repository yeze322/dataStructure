#ifndef __BTREE__
#define __BTREE__

#define KEEP_TRACK 1
#if KEEP_TRACK
#define PTRACK(x) cout<<x
#else
#define PTRACK(x) ;
#endif

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
using std::cout;
using std::cerr;
using std::endl;
using std::vector;

template<typename D>
class BTreeNode{
public:
    /*only leaf node split, new root make tree higher*/
    bool isLeaf;
    /*assuming that we save int in the tree, will promote it*/
    vector<D> key;
    /*child, always 1 more than keys*/
    vector<BTreeNode<D>*> child;
    /*self print function for debugging*/
    void printNode();
    int insertNode(const D&);
    void splitNode();
    /*read without change*/
    D getLeftMostKey() const {return *key.begin();}
    D getRightMostKey() const {return *key.rbegin();}

    /*support pop/push elements from left side or right side*/
    D popLeftMostKey(){ return __vecPopLeft<D>(key);} 
    D popRightMostKey() {return __vecPopRight<D>(key);}
    BTreeNode<D>* popLeftMostChild() { return __vecPopLeft<BTreeNode<D>*>(child);}
    BTreeNode<D>* popRightMostChild() { return __vecPopRight<BTreeNode<D>*>(child);}

    void pushKeyFromLeft(D val) {__vecPushLeft<D>(key,val);}
    void pushKeyFromRight(D val) {__vecPushRight<D>(key,val);}
    void pushChildFromLeft(BTreeNode<D>* x) {__vecPushLeft<BTreeNode<D>*>(child,x);}
    void pushChildFromRight(BTreeNode<D>* x) {__vecPushRight<BTreeNode<D>*>(child,x);}

    /*simple constructor*/
    BTreeNode():isLeaf(true) {}
    /*find the key word's index*/
    int findKeyword(D val){
        auto iter = find(key.begin(), key.end(), val);
        if(iter == key.end()) return -1;
        else return iter - key.begin();
    }
private:
    template<typename T> T __vecPopLeft(vector<T>& vec){
        if(vec.empty()){
            cerr << "In __vecPopLeft, vec is empty."<<endl;   
            exit(1);
        }
        T ret = *vec.begin();
        vec.erase(vec.begin());
        return ret;
    }
    template<typename T> T __vecPopRight(vector<T> &vec){
        if(vec.empty()){
            cerr << "In __vecPopRight, vec is empty."<<endl;
            exit(1);
        }
        T ret = *vec.rbegin();
        vec.pop_back();
        return ret;
    }
    template<typename T> void __vecPushLeft(vector<T>& vec, const T& val) {vec.insert(vec.begin(), val);}
    template<typename T> void __vecPushRight(vector<T>& vec, const T& val) {vec.push_back(val);}
};

/*
when reach the leaf, if keys number in [t-1,2t-1), needn't to split
else, split it. move the middle element to father node.
*/
template<typename D>
int BTreeNode<D>::insertNode(const D& value){
    PTRACK("insertNode()\n");
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
template<typename D>
void BTreeNode<D>::splitNode(){
    PTRACK("splitNode"<<endl);
    int splitIndex = key.size()/2;
    //create two new node
    BTreeNode<D> *left = new BTreeNode<D>();
    BTreeNode<D> *right = new BTreeNode<D>();
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

template<typename D>
void BTreeNode<D>::printNode(){
    PTRACK("printNode()\n");
    BTreeNode<D> mark;
    vector<BTreeNode<D>*> toPrint(1,this);
    while(!toPrint.empty()){
        vector<BTreeNode<D>*> nextLevel;
        for(auto x : toPrint){
            cout <<"[";
            if(x == NULL) cout << "NULL";
            else if(x == &mark){
                cout << "==";
            }
            else{
                cout << "("<<x->child.size();
                if(x->isLeaf) cout <<"L";
                cout <<")";
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
