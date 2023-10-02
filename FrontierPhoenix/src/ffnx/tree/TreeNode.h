#ifndef FRONTIER_PHOENIX_TREE_TREENODE_H
#define FRONTIER_PHOENIX_TREE_TREENODE_H

#include <vector>
#include <memory>

namespace ffnx::tree {

    template <typename TValue>
    class TreeNode {
    private:

        std::vector<std::shared_ptr<TreeNode>> _child_nodes;

        TValue _value;

    public:
        TreeNode(const TValue& value) : _value(value) {

        }

        std::weak_ptr<TreeNode> append_child_node(const TValue& value) {
            auto node = std::make_shared<TreeNode>(value);
            _child_nodes.push_back(node);

            return node;
        }

        const TValue& value() const {
            return _value;
        }

        const std::vector<std::shared_ptr<TreeNode<TValue>>> &visit_child_nodes() const {
            return _child_nodes;
        }

    };

}

#endif