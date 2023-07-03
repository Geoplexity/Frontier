#ifndef FRONTIER_PHOENIX_DRPLAN_PLANS_DEFAULT_H
#define FRONTIER_PHOENIX_DRPLAN_PLANS_DEFAULT_H

#include "drplan/DRPlan.h"
#include "cluster/Cluster.h"

namespace ffnx::drplan {

    template <typename TVert, typename TEdge>
    class DefaultDRPlan : public DRPlan<TVert, TEdge> {
        using FlowGraph = ffnx::flowgraph::FlowGraph<TVert, TEdge>;
        using FlowGraphInterface = ffnx::flowgraph::FlowGraphInterface<TVert, TEdge>;

        using VertDesc = typename FlowGraph::vertex_descriptor;
        using EdgeDesc = typename FlowGraph::edge_descriptor;

    public:
        void solve(ffnx::flowgraph::FlowGraphInterface<TVert, TEdge>& graphInterface) override {
            // todo: start with copy of
            // FrontierPhoenix/canonical-top-down-dr-planner/src/Graph/DR_Plan.cpp

            // graph -> edges

            // original dr plan starts with separate cluster collections:
            // cluster_e, cluster_v

            // generate map edge : set(cluster_e)
            // map edge : (edge : cluster_e)

            // todo: incorporate pebblegame 2D


        }

    };
}

#endif