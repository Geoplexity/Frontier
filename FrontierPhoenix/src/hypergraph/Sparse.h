#ifndef FRONTIER_PHOENIX_HYPERGRAPH_SPARSEHYPERGRAPH_H
#define FRONTIER_PHOENIX_HYPERGRAPH_SPARSEHYPERGRAPH_H

#include "hypergraph/HyperGraph.h"

namespace ffnx::hypergraph {

    class SparseHyperEdge : public HyperEdge {
    private:
        std::set<node_ptr> hyperNodes;

    public:
        void visitNodes(const std::function<void(node_ptr)>& visitor) const {
            for (const auto& node : hyperNodes) {
                visitor(node);
            }
        }

        bool hasNode(node_ptr toAdd) {
            return hyperNodes.contains(toAdd);
        }

        void addNode(node_ptr toAdd) {
            if (hyperNodes.contains(toAdd)) {
                throw std::runtime_error("Node already added.");
            }

            hyperNodes.insert(toAdd);
        }

        void removeNode(node_ptr toRemove) {
            if (!hyperNodes.contains(toRemove)) {
                throw std::runtime_error("Node not contained in edge");
            }

            hyperNodes.erase(toRemove);
        }

    };

    class SparseHyperGraph : public HyperGraph {
    private:
        using edge_ptr_mutable = std::shared_ptr<HyperEdge>;

        std::set<node_ptr> nodes;

        // since edges store pointers to nodes, they must be mutable (nodes can be removed etc.)
        std::set<edge_ptr_mutable> edges;

        [[nodiscard]] node_ptr assertHasNode(const node_ptr_ext &node) const {
            auto ptr = node.lock();

            if (!nodes.contains(ptr)) {
                throw std::runtime_error("Node not contained in graph.");
            }

            return ptr;
        }

        [[nodiscard]] std::shared_ptr<SparseHyperEdge> assertHasEdge(const edge_ptr_ext& edge) const {
            auto ptr = edge.lock();

            if (!edges.contains(ptr)) {
                throw std::runtime_error("Edge not contained in graph.");
            }

            // this is safe, since SparseHyperEdge is the only edge type allowed in the collection.
            return std::dynamic_pointer_cast<SparseHyperEdge>(ptr);
        }

    public:
        [[nodiscard]] bool containsNode(node_ptr_ext node) const override {
            return nodes.contains(node.lock());
        }

        [[nodiscard]] bool containsEdge(edge_ptr_ext edge) const override {
            return edges.contains(edge.lock());
        }

        void visitEdges(const std::function<void (std::shared_ptr<HyperEdge>)> &visitor) const override {
            for (const auto& edge : edges) {
                visitor(edge);
            }
        }

        void visitNodes(const std::function<void (node_ptr)> &visitor) const override {
            for (const auto& node : nodes) {
                visitor(node);
            }
        }

        void visitEdgeNodes(edge_ptr_ext edge,
                            const std::function<void (node_ptr)> &visitor) const override {

            assertHasEdge(edge)->visitNodes(visitor);
        }

        std::weak_ptr<HyperNode> addNode() override {
            auto result = std::make_shared<HyperNode>();

            nodes.insert(result);

            return result;
        }

        void removeNode(std::weak_ptr<HyperNode> toRemove) override {
            auto toRemovePtr = assertHasNode(toRemove);

            nodes.erase(toRemovePtr);

            for (auto &edge : edges) {
                auto edgePtr = std::static_pointer_cast<SparseHyperEdge>(edge);

                if (edgePtr->hasNode(toRemovePtr)) {
                    edgePtr->removeNode(toRemovePtr);
                }
            }
        }

        edge_ptr_ext addEdge() override {
            auto result = std::make_shared<SparseHyperEdge>();

            edges.insert(result);

            return result;
        }

        void removeEdge(std::weak_ptr<HyperEdge> toRemove) override {
            auto toRemovePtr = assertHasEdge(toRemove);

            edges.erase(toRemovePtr);
        }

        void connect(std::weak_ptr<HyperNode> node, edge_ptr_ext edge) override {
            auto nodePtr = assertHasNode(node);
            auto edgePtr = assertHasEdge(edge);

            edgePtr->addNode(nodePtr);
        }

        void disconnect(std::weak_ptr<HyperNode> node, edge_ptr_ext edge) override {
            auto nodePtr = assertHasNode(node);
            auto edgePtr = assertHasEdge(edge);

            edgePtr->removeNode(nodePtr);
        }

    };
}

#endif