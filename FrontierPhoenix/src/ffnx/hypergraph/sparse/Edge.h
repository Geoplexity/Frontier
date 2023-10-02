#ifndef FRONTIER_PHOENIX_HYPERGRAPH_SPARSEHYPEREDGE_H
#define FRONTIER_PHOENIX_HYPERGRAPH_SPARSEHYPEREDGE_H

#include "ffnx/hypergraph/HyperGraph.h"

namespace ffnx::hypergraph {

    class SparseHyperEdge : public HyperEdge {
    private:
        std::set <node_ptr> hyperNodes;

    public:
        void visitNodes(const std::function<void(node_ptr)> &visitor) const;

        [[nodiscard]] bool hasNode(const node_ptr &toAdd) const;

        void addNode(const node_ptr &toAdd);

        void removeNode(const node_ptr &toRemove);

    };
}

#endif