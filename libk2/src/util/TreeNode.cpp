//
// Created by minimal on 12/14/20.
//
#include <cmath>

#include "TreeNode.h"


k2::TreeNode k2::TreeNode::setChildIfAbsent(size_t i, TreeNodeBuffer& treeNodeBuffer){
	TreeNode child_node = treeNodeBuffer.getTreeNodeChild(*this, i);
    if(child_node == NO_CHILD_NODE) {
		treeNodeBuffer.getTreeNodeValue(*this).value +=pow(2, i);
		child_node = treeNodeBuffer.constructTreeNode();
		treeNodeBuffer.getTreeNodeChild(*this, i) = child_node;
    }
    return child_node;
}

void k2::TreeNode::clear(TreeNodeBuffer& treeNodeBuffer) {
	treeNodeBuffer.clearTreeNode(*this);
}

k2::TreeNode k2::TreeNode::getChild(size_t i, TreeNodeBuffer& treeNodeBuffer) {
    return treeNodeBuffer.getTreeNodeChild(*this, i);
}

bool k2::TreeNode::isLeaf(TreeNodeBuffer& treeNodeBuffer) {
    return !treeNodeBuffer.getTreeNodeValue(*this).value;
}

u_char k2::TreeNode::getRawValue(bool reverse, TreeNodeBuffer& treeNodeBuffer) const{
	u_char value =  treeNodeBuffer.getTreeNodeValue(*this).value;
    if (!reverse)
        return value;
    u_char ret = 0;
    ret = ret | ((value & 8) / 8);
    ret = ret | ((value & 4) / 2);
    ret = ret |((value & 2) * 2);
    ret = ret |((value & 1) * 8);
    return ret;
}

k2::TreeNode::TreeNode(uint64_t id) : id(id) {}

k2::TreeNode::operator size_t() const {
	return id;
}

k2::TreeNode::operator bool() const {
	return id != NO_CHILD_NODE;
}
