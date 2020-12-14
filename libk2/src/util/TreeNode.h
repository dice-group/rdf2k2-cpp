//
// Created by minimal on 12/14/20.
//

#ifndef RDF2K2_CPP_TREENODE_H
#define RDF2K2_CPP_TREENODE_H

#include <memory>

using namespace std;

class TreeNode {

public:
    TreeNode setChildIfAbsent(int i, TreeNode &child);

private:
    char value=0;
    TreeNode *parent;
    TreeNode *children[4] {nullptr,nullptr,nullptr,nullptr};
};


#endif //RDF2K2_CPP_TREENODE_H
