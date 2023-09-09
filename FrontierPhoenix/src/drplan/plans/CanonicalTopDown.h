#ifndef FRONTIER_PHOENIX_DRPLAN_PLANS_CANONICAL_TOP_DOWN_H
#define FRONTIER_PHOENIX_DRPLAN_PLANS_CANONICAL_TOP_DOWN_H

#include "flowgraph/FlowGraph.h"
#include "drplan/DRPlanner.h"
#include "cluster/Cluster.h"
#include "tree/TreeNode.h"
#include "drplan/plans/pebble_game/PebbleGame2D.h"

namespace ffnx::drplan {

    template <typename TVert, typename TEdge>
    class CanonicalTopDown : public DRPlanner<TVert, TEdge> {
        using FlowGraph = ffnx::flowgraph::FlowGraph<TVert, TEdge>;
        using FlowGraphInterface = ffnx::flowgraph::FlowGraphInterface<TVert, TEdge>;
        using Cluster = ffnx::cluster::Cluster<TVert, TEdge>;
        using TreeNode = ffnx::tree::TreeNode<std::shared_ptr<Cluster>>;

        using VertDesc = typename FlowGraph::vertex_descriptor;
        using EdgeDesc = typename FlowGraph::edge_descriptor;

    public:
        DRPlan<TVert, TEdge> solve(std::shared_ptr<const ffnx::flowgraph::FlowGraphInterface<TVert, TEdge>> graph) override {
            // based on:
            // FrontierPhoenix/canonical-top-down-dr-planner/src/Graph/DR_Plan.cpp

            // starts with root solution tree node: full graph (verts and edges)
            auto result = DRPlan<TVert, TEdge>::Builder(graph);

            for (const auto &e : graph_interface.graph()->edges()) {

                // constructs a sub_cluster (root cluster minus a single edge)
                auto sub_cluster = result.root().lock()->get_edge_filtered_cluster([&e](const auto &e_excluded){
                    return e_excluded != e;
                });

                // add this sub-cluster to the solution root
                result.add_branch(result.root(), sub_cluster);

                auto game_result = ffnx::pebblegame::PebbleGame2D<VertDesc, EdgeDesc>(sub_cluster).run();

            }



        }

    };
}

#endif