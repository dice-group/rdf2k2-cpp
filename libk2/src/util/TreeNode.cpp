//
// Created by minimal on 12/14/20.
//
#include "math.h"

#include "TreeNode.h"


unique_ptr<TreeNode> &TreeNode::setChildIfAbsent(int i){
    if(not (children[i])) {
        value+=pow(2, i);
        children[i] = make_unique<TreeNode>();
    }
    return children[i];
}

void TreeNode::clear() {
    children[0] = nullptr;
    children[1] = nullptr;
    children[2] = nullptr;
    children[3] = nullptr;
}

unique_ptr<TreeNode> &TreeNode::getChild(int i) {
    return children[i];

}

unsigned char TreeNode::getRawValue(bool reverse) const{
    if (!reverse)
        return value;
    unsigned char ret = 0;
    ret = ret | ((value & 8) / 8);
    ret = ret | ((value & 4) / 2);
    ret = ret |((value & 2) * 2);
    ret = ret |((value & 1) * 8);
    return ret;
}