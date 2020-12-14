//
// Created by minimal on 12/14/20.
//
#include "math.h"

#include "TreeNode.h"

TreeNode TreeNode::setChildIfAbsent(int i, TreeNode &child){
    if(children[i]==nullptr) {
        value+=pow(2, i);
        children[i] = &child;
    }
    return *children[i];
}