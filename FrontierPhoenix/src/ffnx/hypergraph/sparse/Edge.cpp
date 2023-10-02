#include "ffnx/hypergraph/sparse/Edge.h"

namespace ffnx::hypergraph {

    void SparseHyperEdge::visitNodes(const std::function<void(node_ptr)> &visitor) const {
        for (const auto &node: hyperNodes) {
            visitor(node);
        }
    }

    bool SparseHyperEdge::hasNode(const node_ptr &toAdd) const {
        return hyperNodes.contains(toAdd);
    }

    void SparseHyperEdge::addNode(const node_ptr &toAdd) {
        if (hyperNodes.contains(toAdd)) {
            throw std::runtime_error("Node already added.");
        }

        hyperNodes.insert(toAdd);
    }

    void SparseHyperEdge::removeNode(const node_ptr &toRemove) {
        if (!hyperNodes.contains(toRemove)) {
            throw std::runtime_error("Node not contained in edge");
        }

        hyperNodes.erase(toRemove);
    }


}