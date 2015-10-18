#include "tree.h"

int main(){
    BTreeNode<int>* root = new BTreeNode<int>();
    for(int i=0;i<5;i++){
        root->insertNode(i);
    }
    root->printNode();
}
