//
// Created by minimal on 12/14/20.
//

#ifndef RDF2K2_CPP_TREENODE_H
#define RDF2K2_CPP_TREENODE_H

#include <memory>
#include <vector>

using namespace std;

class TreeNode {

public:
    class TreeNodeBuffer {
        vector<TreeNode> buffer;
        size_t position = 0;
    public:
        explicit TreeNodeBuffer(size_t initialSize = 10'000L) {
            buffer.reserve(initialSize);
        }

        [[maybe_unused]] TreeNode &constructTreeNode() {
            buffer.push_back(TreeNode{});
            return buffer[position++];
        }

        [[maybe_unused]] TreeNode &getTreeNode(const size_t pos) {
            return buffer[pos];
        }

        [[nodiscard]] size_t lastPos() const  {
            return position-1;
        }
    };

    TreeNode *setChildIfAbsent(int i, TreeNodeBuffer& treeNodeBuffer);

    TreeNode *getChild(int i, TreeNodeBuffer& treeNodeBuffer);

    void clear();

    [[nodiscard]] unsigned char getRawValue(bool reverse) const;

    bool isLeaf();

private:
    static constexpr size_t NO_CHILD_NODE = std::numeric_limits<size_t>::max();
    // uint32_t statt size_t könnte statt size_t verwendet werden um den Speicherbedarf auf ca. 3/5 zu senken
    size_t children[4]{NO_CHILD_NODE, NO_CHILD_NODE, NO_CHILD_NODE, NO_CHILD_NODE};
    // Für maximales Speicher sparen müsste das hier in ein eigenes Array ausgelagert werden.
    unsigned char value = 0;

    //11000000 00110000 00001100 00000011

};


#endif //RDF2K2_CPP_TREENODE_H
