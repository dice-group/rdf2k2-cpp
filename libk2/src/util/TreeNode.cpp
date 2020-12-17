//
// Created by minimal on 12/14/20.
//
#include "math.h"

#include "TreeNode.h"


TreeNode *TreeNode::setChildIfAbsent(int i, TreeNodeBuffer& treeNodeBuffer){
    if(children[i] == NO_CHILD_NODE) {
        value+=pow(2, i);
        &treeNodeBuffer.constructTreeNode();
        children[i] = treeNodeBuffer.lastPos();
    }
    return &treeNodeBuffer.getTreeNode(children[i]);
}

void TreeNode::clear() {
    children[0] = NO_CHILD_NODE;
    children[1] = NO_CHILD_NODE;
    children[2] = NO_CHILD_NODE;
    children[3] = NO_CHILD_NODE;
}

TreeNode *TreeNode::getChild(int i, TreeNodeBuffer& treeNodeBuffer) {
    if(children[i] == NO_CHILD_NODE){
        return nullptr;
    }
    return &treeNodeBuffer.getTreeNode(children[i]);
}

bool TreeNode::isLeaf() {
    return !value;
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