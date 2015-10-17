#include "tree.h"
#include "node.h"
#include <string>
using namespace std;
void insertGenerateTree(){
    BTreeNode *root = new BTreeNode();    
    string input = "FSQKCLHTVWMRNPABXY";
    cout <<"test case = {";
    for(auto x : input) cout << (int)x <<",";
    cout <<"}\n";
    cout << "start test"<<endl;
    for(auto x : input) B_TREE_INSERT(root, x);
    cout <<"finish!"<<endl;
    root->printNode();
}

BTreeNode* generateTest(){
    cout << "before"<<endl;
    vector<string> input = {"P", "CGM", "TX", "AB", "DEF","JKL","NO","QMS","UV","YZ"};
    cout << "after"<<endl;
    BTreeNode* root = B_TREE_GENERATE(input);
    root->printNode();
    return root;
}

void treeTest(){
    BTreeNode *root = generateTest();
    cout << "\n\n\ntest DELETE"<<endl;
    char val;
    while(1){
        cout << "input a char to delete: ";
        cin >> val;
        cout << "="<<int(val);
        B_TREE_DELETE(root, (int)val);
        if(root->key.empty()){
            BTreeNode *temp = root;
            root = root->child[0];
            delete temp;
        }
        root->printNode();
    }
}

void nodeTest(){
    cout <<"Node test Begin!\n"<<endl;
    BTreeNode *root = new BTreeNode();
    for(int i=0;i<5;i++){
        root->insertNode(i);
    }
    root->splitNode();
    root->printNode();
    root->pushKeyFromLeft(33);
    root->pushKeyFromRight(44);
    root->pushChildFromLeft(NULL);
    root->printNode();
}

int main(){
    treeTest();
}
