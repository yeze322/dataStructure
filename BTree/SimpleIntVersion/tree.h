#ifndef __TREE__
#define __TREE__

#include "node.h"
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <string>
using namespace std;
static int MINIMUM_DEGREE = 3;

BTreeNode* B_TREE_GENERATE(vector<string>& input){
    PTRACK("Generate Tree -- Basic Node:"<<input.size()<<endl);
    vector<BTreeNode*> nodelist;
    for(auto & x : input){
        BTreeNode *temp = new BTreeNode();
        temp->isLeaf = false;
        for(auto c : x) temp->key.push_back((int)c);
        nodelist.push_back(temp);
    }
    PTRACK("\tConnecting"<<endl);
    int addIndex = 0;
    int visit = 1;
    while(visit<nodelist.size()){
        int childNum = nodelist[addIndex]->key.size()+1;
        for(int i=0;i<childNum;i++){
            nodelist[addIndex]->child.push_back(nodelist[visit]);
            ++visit;
        }
        PTRACK("\t\tSucess, connet: "<<childNum<<" to the "<<addIndex<<" node, visit = "<<visit<<endl);
        ++addIndex;
    }
    while(addIndex<nodelist.size()){
        nodelist[addIndex]->isLeaf = true;
        addIndex++;
    }
    return nodelist[0];
}

pair<BTreeNode*, int> B_TREE_SEARCH(BTreeNode *root, int val){
    auto & keyList = root->key;
    int i=0;
    for(i=0;i<keyList.size();i++){
       if(val == keyList[i]) return {root, i};
       else if(val < keyList[i]) break;
    }
    //no match, need to move pointer.
    if(root->isLeaf) return {NULL, -1};
    return B_TREE_SEARCH(root->child[i], val);
}

void B_TREE_SPLIT_CHILD(BTreeNode *root, int index){
    PTRACK("B_TREE_SPLIT_CHILD: " << root << " at " << index << endl);
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
    PTRACK("print Tree:\n"; root->printNode(););
}

void B_TREE_INSERT_NONFULL(BTreeNode *root, int val){
    PTRACK("B_TREE_INSERT_NONFULL: "<< root << " add "<<val<<endl);
    auto & key_ref = root->key;
    //find the first ele which is bigger than val
    int i=0;
    while(i<key_ref.size() && val > key_ref[i]) ++i;
    //what if val == key??
    if(i<key_ref.size() && val == key_ref[i]) return;

    if(root->isLeaf){
        key_ref.insert(key_ref.begin() + i, val);
    }else{
        if(root->child[i]->key.size() == 2*MINIMUM_DEGREE -1){//full node
            B_TREE_SPLIT_CHILD(root, i);
            if(key_ref[i] < val) ++i;
            else if(key_ref[i] == val) return;
        }
        B_TREE_INSERT_NONFULL(root->child[i], val);
    }
};

void B_TREE_INSERT(BTreeNode* & root, int val){
    //only when root is full, will tree create a new root to grow higher
    //the split operation is different from split in non-root. because of the absence of root's root
    PTRACK("B_TREE_INSERT: "<<root<<","<<val<<endl);
    if(root->key.size() == 2*MINIMUM_DEGREE-1){
       BTreeNode *newRoot = new BTreeNode();
       newRoot->isLeaf = false;
       newRoot->child.push_back(root);
       B_TREE_SPLIT_CHILD(newRoot, 0);
       root = newRoot;
    }
    B_TREE_INSERT_NONFULL(root, val);
}

void B_TREE_MERGE_CHILD(BTreeNode *root, int keyIndex){
    if(keyIndex >= root->key.size()){
        cerr << "Errpr when called B_TREE_MERGE_CHILD, index out of range!"<<endl;
        exit(1);
    }
    BTreeNode *lchild = root->child[keyIndex];
    BTreeNode *rchild = root->child[keyIndex+1];
    //merge key: this.child[index].key + key[index] + this.child[index+1].key -> child[index].key
    lchild->key.push_back(root->key[keyIndex]);
    lchild->key.insert(lchild->key.end(), rchild->key.begin(), rchild->key.end());
    root->key.erase(root->key.begin()+keyIndex);
    root->child.erase(root->child.begin()+keyIndex+1);
    //merge child: this.child[index].child + this.child[index+1].child -> child[Index].child
    if(!lchild->isLeaf){
        lchild->child.insert(lchild->child.end(), rchild->child.begin(), rchild->child.end());
    }
    delete rchild;
    PTRACK("B_TREE_MERGE_CHILD:\n";root->printNode(););
}

void B_TREE_DELETE(BTreeNode *root, int val){
    PTRACK("call B_TREE_DELETE!\n");
    //unless root is a leaf node, it must have more than MINIMUM_DEGREE - 1 keywords.
    int index = root->findKeyword(val);
    if(index!=-1){//found it
        PTRACK("\tDELETE, condition FOUND\n");
        if(root->isLeaf){
            root->key.erase(root->key.begin()+index);
            PTRACK("\t\tisLeaf, delete SUCESS!\n");
        }else{
            PTRACK("\t\tnot leaf, adjusting:\n");
            BTreeNode *y = root->child[index];
            BTreeNode *z = root->child[index+1];
            if(y->key.size() >= MINIMUM_DEGREE){
                //swap key's precede val in left and himself.
                auto precedeVal = y->getRightMostKey();
                root->key[index] = precedeVal;
                PTRACK("\t\t\tSwap with left bro\n"; root->printNode(););

                B_TREE_DELETE(y, precedeVal);
                return;
            }else if(z->key.size() >= MINIMUM_DEGREE){
                //swap key's follow val in right and himself
                auto followVal = z->getLeftMostKey();
                root->key[index] = followVal;
                PTRACK("\t\t\tSwap with right bro\n"; root->printNode(););

                B_TREE_DELETE(z, followVal);
                return;
            }else{
                B_TREE_MERGE_CHILD(root, index);
                PTRACK("\t\t\tMerge Bro\n"; root->printNode(););

                B_TREE_DELETE(root->child[index], val);
                return;
            }
        }
    }else{//didn't found val
        PTRACK("\tDELETE: Condition Not found!\n");
        if(root->isLeaf){
            PTRACK("\t\tFAILED! Delete value: "<<val<<"not found!"<<endl);
        }else{
            PTRACK("\t\tDELETE down, adjusting:\n");
            int childIndex = 0;
            while(childIndex < root->key.size() && root->key[childIndex] < val) ++childIndex;
            //now value must be in child[childIndex]
            BTreeNode *nextRoot = root->child[childIndex];
            if(nextRoot->key.size() >= MINIMUM_DEGREE){
                PTRACK("\t\t\tGO DOWN!\n");
                //nextRoot have more than MINIMUM_DEGREE children, recursive-descent
                B_TREE_DELETE(nextRoot, val);
                return;
            }else{
                //chidRoor have less than MINIMUM_DEGREE children, adjust it before recursive-descent
                BTreeNode *leftBro = NULL;
                BTreeNode *rightBro = NULL;
                if(childIndex != 0){
                    //examine left bro
                    leftBro = root->child[childIndex-1];   
                    if(leftBro->key.size() >= MINIMUM_DEGREE){
                        auto borrowedKey = leftBro->popRightMostKey();
                        auto key_RootTransDown = root->key[childIndex-1];
                        root->key[childIndex-1] = borrowedKey;
                        nextRoot->pushKeyFromLeft(key_RootTransDown);

                        if(!nextRoot->isLeaf){
                            auto borrowedChild = leftBro->popRightMostChild();
                            nextRoot->pushChildFromLeft(borrowedChild);
                        }
                        PTRACK("\t\t\tBorrow from left bro\n"; root->printNode(););

                        B_TREE_DELETE(nextRoot, val);
                        return;
                    }
                }
                if(childIndex != root->child.size() - 1){
                    //examine right brother
                    rightBro = root->child[childIndex+1];
                    if(rightBro->key.size() >= MINIMUM_DEGREE){
                        auto borrowedKey = rightBro->popLeftMostKey();
                        auto key_RootTransDown = root->key[childIndex];
                        root->key[childIndex] = borrowedKey;
                        nextRoot->pushKeyFromRight(key_RootTransDown);

                        if(!nextRoot->isLeaf){
                            auto borrowedChild = rightBro->popLeftMostChild();
                            nextRoot->pushChildFromRight(borrowedChild);
                        }
                        PTRACK("\t\t\tBorrow fom right bro\n"; root->printNode(););

                        B_TREE_DELETE(nextRoot, val);
                        return;
                    }
                }
                //else, neither rbro and lbro >= MINIMUM_DEGREE, merge them.
                //follow a simple rule: if lbro not NULL, always merge lbro and root rather than rbro.
                if(leftBro == NULL && rightBro == NULL){
                    cerr << "in B_TREE_DELETE, tail, leftBro and rightBro both NULL, impossible!" << endl;
                    exit(1);
                }
                PTRACK("\t\t\tMerge tow brow, recursive down\n");
                if(leftBro != NULL){
                    B_TREE_MERGE_CHILD(root, childIndex-1);
                    B_TREE_DELETE(root->child[childIndex-1], val);
                    return;
                }else{
                    B_TREE_MERGE_CHILD(root, childIndex);
                    B_TREE_DELETE(root->child[childIndex], val);
                    return;
                }
            }
        }
    }
}
#endif
