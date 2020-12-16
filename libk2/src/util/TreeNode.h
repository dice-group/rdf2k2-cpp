//
// Created by minimal on 12/14/20.
//

#ifndef RDF2K2_CPP_TREENODE_H
#define RDF2K2_CPP_TREENODE_H

#include <memory>

using namespace std;

class TreeNode {

public:

    unique_ptr<TreeNode> &setChildIfAbsent(int i);

    unique_ptr<TreeNode> &getChild(int i);

    void clear();

    [[nodiscard]] unsigned char getRawValue(bool reverse) const;

private:
    char value = 0;
    unique_ptr<TreeNode> children[4]{nullptr, nullptr, nullptr, nullptr};

};


#endif //RDF2K2_CPP_TREENODE_H
