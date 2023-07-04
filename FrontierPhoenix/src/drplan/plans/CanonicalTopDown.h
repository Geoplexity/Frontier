#ifndef FRONTIER_PHOENIX_DRPLAN_PLANS_DEFAULT_H
#define FRONTIER_PHOENIX_DRPLAN_PLANS_DEFAULT_H

#include "flowgraph/FlowGraph.h"
#include "drplan/DRPlan.h"
#include "cluster/Cluster.h"
#include "tree/TreeNode.h"
#include "drplan/plans/pebble_game/PebbleGame2D.h"

namespace ffnx::drplan {

    template <typename TVert, typename TEdge>
    class DefaultDRPlan : public DRPlan<TVert, TEdge> {
        using FlowGraph = ffnx::flowgraph::FlowGraph<TVert, TEdge>;
        using FlowGraphInterface = ffnx::flowgraph::FlowGraphInterface<TVert, TEdge>;
        using Cluster = ffnx::cluster::Cluster<TVert, TEdge>;
        using TreeNode = ffnx::tree::TreeNode<std::shared_ptr<Cluster>>;

        using VertDesc = typename FlowGraph::vertex_descriptor;
        using EdgeDesc = typename FlowGraph::edge_descriptor;

    public:
        void solve(ffnx::flowgraph::FlowGraphInterface<TVert, TEdge>& graph_interface) override {
            // todo: start with copy of
            // FrontierPhoenix/canonical-top-down-dr-planner/src/Graph/DR_Plan.cpp

            auto cluster_builder = Cluster::Builder(graph_interface.graph());
            for (const auto& e: graph_interface.graph()->edges()) {
                cluster_builder.add_edge(e);
            }

            TreeNode solution_tree(cluster_builder.build());

            for (const auto& e: graph_interface.graph()->edgea()) {
                auto sub_cluster = solution_tree.value()->get_edge_filtered_cluster(
                        [&e](const auto& ee){ return e != ee; });

                auto sub_tree_node = solution_tree.append_child_node(sub_cluster);

                // further divide this node via pebble-game into
                // sub clusters
                ffnx::pebblegame::PebbleGame2D<VertDesc, EdgeDesc> pg(
                        sub_tree_node.lock()->value());

                auto sub_sub_clusters = pg.run_component();
            }

            // original dr plan starts with separate cluster collections:
            // cluster_e, cluster_v

            // generate map edge : set(cluster_e)
            // map edge : (edge : cluster_e)

            // todo: incorporate pebblegame 2D


        }

    };
}

#endif