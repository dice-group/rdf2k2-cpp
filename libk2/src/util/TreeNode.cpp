//
// Created by minimal on 12/14/20.
//
#include <cmath>

#include "TreeNode.h"


TreeNode TreeNode::setChildIfAbsent(int i, TreeNodeBuffer& treeNodeBuffer){
	TreeNode child_node = treeNodeBuffer.getTreeNodeChild(*this, i);
    if(child_node == NO_CHILD_NODE) {
		treeNodeBuffer.getTreeNodeValue(*this).value +=pow(2, i);
		child_node = treeNodeBuffer.constructTreeNode();
		treeNodeBuffer.getTreeNodeChild(*this, i) = child_node;
    }
    return child_node;
}

void TreeNode::clear(TreeNodeBuffer& treeNodeBuffer) {
	treeNodeBuffer.clearTreeNode(*this);
}

TreeNode TreeNode::getChild(int i, TreeNodeBuffer& treeNodeBuffer) {
    return treeNodeBuffer.getTreeNodeChild(*this, i);
}

bool TreeNode::isLeaf(TreeNodeBuffer& treeNodeBuffer) {
    return !treeNodeBuffer.getTreeNodeValue(*this).value;
}

unsigned char TreeNode::getRawValue(bool reverse, TreeNodeBuffer& treeNodeBuffer) const{
	char value =  treeNodeBuffer.getTreeNodeValue(*this).value;
    if (!reverse)
        return value;
    unsigned char ret = 0;
    ret = ret | ((value & 8) / 8);
    ret = ret | ((value & 4) / 2);
    ret = ret |((value & 2) * 2);
    ret = ret |((value & 1) * 8);
    return ret;
}

TreeNode::TreeNode(uint64_t id) : id(id) {}

TreeNode::operator size_t() const {
	return id;
}

TreeNode::operator bool() const {
	return id != NO_CHILD_NODE;
}
