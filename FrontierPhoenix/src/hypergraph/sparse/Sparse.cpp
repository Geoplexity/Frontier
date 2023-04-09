#include "Sparse.h"

namespace ffnx::hypergraph {

    node_ptr SparseHyperGraph::assertHasNode(const node_ptr_ext &node) const {
        auto ptr = node.lock();

        if (!nodes.contains(ptr)) {
            throw std::runtime_error("Node not contained in graph.");
        }

        return ptr;
    }

    std::shared_ptr<SparseHyperEdge> SparseHyperGraph::assertHasEdge(const edge_ptr_ext &edge) const {
        auto ptr = edge.lock();

        if (!edges.contains(ptr)) {
            throw std::runtime_error("Edge not contained in graph.");
        }

        // this is safe, since SparseHyperEdge is the only edge type allowed in the collection.
        return std::dynamic_pointer_cast<SparseHyperEdge>(ptr);
    }

    bool SparseHyperGraph::containsNode(node_ptr_ext node) const {
        return nodes.contains(node.lock());
    }

    bool SparseHyperGraph::containsEdge(edge_ptr_ext edge) const {
        return edges.contains(edge.lock());
    }

    void SparseHyperGraph::visitEdges(
            const std::function<void(std::shared_ptr<HyperEdge>)> &visitor) const {
        for (const auto &edge: edges) {
            visitor(edge);
        }
    }

    void SparseHyperGraph::visitNodes(const std::function<void(node_ptr)> &visitor) const {
        for (const auto &node: nodes) {
            visitor(node);
        }
    }

    std::weak_ptr<HyperNode> SparseHyperGraph::addNode() {
        auto result = std::make_shared<HyperNode>();

        nodes.insert(result);

        return result;
    }

    void SparseHyperGraph::removeNode(std::weak_ptr<HyperNode> toRemove) {
        auto toRemovePtr = assertHasNode(toRemove);

        nodes.erase(toRemovePtr);

        for (auto &edge: edges) {
            auto edgePtr = std::static_pointer_cast<SparseHyperEdge>(edge);

            if (edgePtr->hasNode(toRemovePtr)) {
                edgePtr->removeNode(toRemovePtr);
            }
        }
    }

    edge_ptr_ext SparseHyperGraph::addEdge() {
        auto result = std::make_shared<SparseHyperEdge>();

        edges.insert(result);

        return result;
    }

    void SparseHyperGraph::removeEdge(std::weak_ptr<HyperEdge> toRemove) {
        auto toRemovePtr = assertHasEdge(toRemove);

        edges.erase(toRemovePtr);
    }

    void SparseHyperGraph::connect(std::weak_ptr<HyperNode> node, edge_ptr_ext edge) {
        auto nodePtr = assertHasNode(node);
        auto edgePtr = assertHasEdge(edge);

        edgePtr->addNode(nodePtr);
    }

    void SparseHyperGraph::disconnect(std::weak_ptr<HyperNode> node, edge_ptr_ext edge) {
        auto nodePtr = assertHasNode(node);
        auto edgePtr = assertHasEdge(edge);

        edgePtr->removeNode(nodePtr);
    }

}
