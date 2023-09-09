#ifndef FRONTIER_PHOENIX_DRPLAN_PLANS_CANONICAL_TOP_DOWN_H
#define FRONTIER_PHOENIX_DRPLAN_PLANS_CANONICAL_TOP_DOWN_H

#include "drplan/DRPlan.h"

namespace ffnx::drplan {

    template <typename TVert, typename TEdge>
    class MFA : public DRPlan<TVert, TEdge> {
        using FlowGraph = ffnx::flowgraph::FlowGraph<TVert, TEdge>;
        using FlowGraphInterface = ffnx::flowgraph::FlowGraphInterface<TVert, TEdge>;
        using Cluster = ffnx::cluster::Cluster<TVert, TEdge>;
        using TreeNode = ffnx::tree::TreeNode<std::shared_ptr<Cluster>>;

        using VertDesc = typename FlowGraph::vertex_descriptor;
        using EdgeDesc = typename FlowGraph::edge_descriptor;

    public:
        void solve(ffnx::flowgraph::FlowGraphInterface<TVert, TEdge>& graph_interface) override {
            throw std::runtime_error("Not yet implemented");
        }

    };
}

#endif