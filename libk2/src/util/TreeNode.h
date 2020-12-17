//
// Created by minimal on 12/14/20.
//

#ifndef RDF2K2_CPP_TREENODE_H
#define RDF2K2_CPP_TREENODE_H

#include <memory>
#include <vector>
#include <cstdio>
#include <cstdint>

using namespace std;


class TreeNode {
public:
	static constexpr size_t NO_CHILD_NODE = (1UL << 48) - 1;
	TreeNode() = default;

	TreeNode(size_t id);

	operator size_t() const;

	explicit operator bool() const;

	struct ValueType {
		uint8_t value: 2 = 0;
	} __attribute__((packed));

    class TreeNodeBuffer {
    	std::array<std::vector<TreeNode>,4> children_buffer;
		std::vector<ValueType> values_buffer;
        size_t position = 0;
    public:
        explicit TreeNodeBuffer(size_t initialSize = 8L) {
			for (auto &child_buffer  : children_buffer)
				child_buffer.reserve(initialSize);
			values_buffer.reserve(initialSize);
        }

        [[maybe_unused]] TreeNode constructTreeNode() {
        	for (auto &child_buffer  : children_buffer)
				child_buffer.emplace_back();
			values_buffer.emplace_back();
            return position++;
        }

        void clearTreeNode(TreeNode treeNode) {
			for (auto &child_buffer  : children_buffer)
				child_buffer[treeNode] = NO_CHILD_NODE;
			values_buffer[treeNode].value = 0;
        }

        [[maybe_unused]] TreeNode &getTreeNodeChild(TreeNode treeNode, const size_t i) {
            return children_buffer[i][treeNode];
        }

		[[maybe_unused]] ValueType &getTreeNodeValue(TreeNode treeNode) {
			return values_buffer[treeNode];
		}

        [[nodiscard]] size_t lastPos() const  {
            return position-1;
        }
    };

    TreeNode setChildIfAbsent(int i, TreeNodeBuffer& treeNodeBuffer);

    TreeNode getChild(int i, TreeNodeBuffer& treeNodeBuffer);

    void clear(TreeNodeBuffer& treeNodeBuffer);

    [[nodiscard]] unsigned char getRawValue(bool reverse, TreeNodeBuffer& treeNodeBuffer) const;

    bool isLeaf(TreeNodeBuffer& treeNodeBuffer);

private:
	size_t id: 48 = NO_CHILD_NODE;

} __attribute__((packed));


#endif //RDF2K2_CPP_TREENODE_H
