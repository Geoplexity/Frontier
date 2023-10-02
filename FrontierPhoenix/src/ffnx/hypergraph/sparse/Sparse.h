#ifndef FRONTIER_PHOENIX_HYPERGRAPH_SPARSEHYPERGRAPH_H
#define FRONTIER_PHOENIX_HYPERGRAPH_SPARSEHYPERGRAPH_H

#include "ffnx/hypergraph/HyperGraph.h"
#include "ffnx/hypergraph/sparse/Edge.h"

namespace ffnx::hypergraph {

    class SparseHyperGraph : public HyperGraph {
    private:
        using edge_ptr_mutable = std::shared_ptr<HyperEdge>;

        std::set<node_ptr> nodes;

        // since edges store pointers to nodes, they must be mutable (nodes can be removed etc.)
        std::set<edge_ptr_mutable> edges;

        [[nodiscard]] node_ptr assertHasNode(const node_ptr_ext &node) const;

        [[nodiscard]] std::shared_ptr<SparseHyperEdge> assertHasEdge(const edge_ptr_ext& edge) const;

    public:
        [[nodiscard]] bool containsNode(node_ptr_ext node) const override;

        [[nodiscard]] bool containsEdge(edge_ptr_ext edge) const override;

        void visitEdges(const std::function<void (std::shared_ptr<HyperEdge>)> &visitor) const override;

        void visitNodes(const std::function<void (node_ptr)> &visitor) const override;

        void visitEdgeNodes(edge_ptr_ext edge,
                            const std::function<void (node_ptr)> &visitor) const override {

            assertHasEdge(edge)->visitNodes(visitor);
        }

        std::weak_ptr<HyperNode> addNode() override;

        void removeNode(std::weak_ptr<HyperNode> toRemove) override;

        edge_ptr_ext addEdge() override;

        void removeEdge(std::weak_ptr<HyperEdge> toRemove) override;

        void connect(std::weak_ptr<HyperNode> node, edge_ptr_ext edge) override;

        void disconnect(std::weak_ptr<HyperNode> node, edge_ptr_ext edge) override;

    };
}

#endif