//
// Created by minimal on 12/14/20.
//
#include "math.h"

#include "TreeNode.h"

shared_ptr<TreeNode> TreeNode::setChildIfAbsent(int i, shared_ptr<TreeNode> &child){
    if(children[i]==nullptr) {
        value+=pow(2, i);
        children[i] = child;
    }
    return children[i];
}

void TreeNode::clear() {
    children[0] = nullptr;
    children[1] = nullptr;
    children[2] = nullptr;
    children[3] = nullptr;
}

shared_ptr<TreeNode> TreeNode::getChild(int i) {
    return children[i];

}

unsigned char TreeNode::getRawValue(bool reverse) {
    if (!reverse)
        return value;
    unsigned char ret = ret | ((value & 8) / 8);
    ret = ret | ((value & 4) / 2);
    ret = ret |((value & 2) * 2);
    ret = ret |((value & 1) * 8);
    return ret;
}