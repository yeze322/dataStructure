#include "tree.h"
#include "node.h"
#include <string>
using namespace std;

BTreeNode* nodeTest(){
    BTreeNode *n = new BTreeNode();
    n->isLeaf = true;
    for(int i=0;i<3;i++){
        n->insertNode(i*5);
    }
    n->splitNode();
    for(int i=1;i<5;i++) n->child[0]->insertNode(i);
    n->printNode();
    return n;
}

void treeTest(){
    auto root = nodeTest();
    auto ret = B_TREE_SEARCH(root, 4);
    cout << "[Search test]: "<<ret.first <<"," << ret.second<<endl;
    B_TREE_SPLIT_CHILD(root, 0);
    root->printNode();
}

int main(){
    BTreeNode *root = new BTreeNode();    
    string input = "FSQKCLHTVWMRNPABXY";
    for(auto x : input) B_TREE_INSERT(root, x);
}
