//
// Created by minimal on 12/14/20.
//

#ifndef RDF2K2_CPP_TREENODE_H
#define RDF2K2_CPP_TREENODE_H

#include <memory>

using namespace std;

class TreeNode {

public:
    shared_ptr<TreeNode> setChildIfAbsent(int i, shared_ptr<TreeNode> child);
    shared_ptr<TreeNode> getChild(int i);
    void clear();
    unsigned char getRawValue(bool reverse);
private:
    char value=0;
    shared_ptr<TreeNode> parent;
    shared_ptr<TreeNode> children[4] {nullptr,nullptr,nullptr,nullptr};

};


#endif //RDF2K2_CPP_TREENODE_H
