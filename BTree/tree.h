#ifndef __TREE__
#define __TREE__

#include "node.h"
#include <vector>
#include <iostream>
using namespace std;

pair<BTreeNode*, int> B_TREE_SEARCH(BTreeNode *root, int val){
    auto & keyList = root->key;
    int i;
    for(i=0;i<keyList.size();i++){
       if(val == keyList[i]) return {root, i};
       else if(val < keyList[i]) break;
    }
    //no match, need to move pointer.
    if(root->isLeaf) return {NULL, -1};
    return B_TREE_SEARCH(root->child[i], val);
}

void B_TREE_SPLIT_CHILD(BTreeNode *root, int index){
    PTRACK << "B_TREE_SPLIT_CHILD " << root << " at " << index << endl;
    BTreeNode *subchild = new BTreeNode();
    BTreeNode *origin = root->child[index];
    subchild->isLeaf = origin->isLeaf;
    //cal the split point and save value in this point
    int middle = origin->key.size()/2;
    auto updateKey = origin->key[middle];
    //copy key & node
    auto iter_keyCopyBegin = origin->key.begin()+middle+1;
    auto iter_keyCopyEnd = origin->key.end();
    subchild->key.insert(subchild->key.end(),iter_keyCopyBegin, iter_keyCopyEnd);
    origin->key.erase(iter_keyCopyBegin-1, iter_keyCopyEnd);
    //copy child node if it's NOT a leaf node
    if(!origin->isLeaf){
        auto iter_childCopyBegin = origin->child.begin()+middle+1;
        auto iter_childCopyEnd = origin->child.end();
        subchild->child.insert(subchild->child.end(), iter_childCopyBegin, iter_childCopyEnd);
        origin->child.erase(iter_childCopyBegin, iter_childCopyEnd);
    }
    //deal with updated key
    root->key.insert(root->key.begin()+index, updateKey);
    root->child.insert(root->child.begin()+index+1, subchild);
}

void B_TREE_INSERT_NONFULL(BTreeNode *root, int val){
    auto & key_ref = root->key;
    //find the first ele which is bigger than val
    int i;
    while(i<key_ref.size() && val < key_ref[i]) ++i;
    //what if val == key??
    if(i<key_ref.size() && val == key_ref[i]) return;

    if(root->isLeaf){
        key_ref.insert(key_ref.begin() + i, val);
    }else{
        BTreeNode *nextRoot = root->child[i];
        if(nextRoot->key.size() == 2*MINIMUM_DEGREE -1){//full node
            B_TREE_SPLIT_CHILD(root, i);
            if(key_ref[i] < val) ++i;
            else if(key_ref[i] == val) return;
        }
        B_TREE_INSERT_NONFULL(nextRoot, val);
    }
};

void B_TREE_INSERT(BTreeNode* & root, int val){
    //only when root is full, will tree create a new root to grow higher
    //the split operation is different from split in non-root. because of the absence of root's root
    if(root->key.size() == 2*MINIMUM_DEGREE-1){
       BTreeNode *newRoot = new BTreeNode();
       newRoot->isLeaf = false;
       newRoot->child.push_back(root);
       B_TREE_SPLIT_CHILD(newRoot, 0);
       root = newRoot;
    }
    B_TREE_INSERT_NONFULL(root, val);
}

#endif
