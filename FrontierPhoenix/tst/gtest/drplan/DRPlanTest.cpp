#include <gtest/gtest.h>
#include <boost/graph/adjacency_list.hpp>
#include "ffnx/cluster/Cluster.h"
#include "ffnx/drplan/DRPlan.h"

TEST(DRPlanTest, testCreate)
{
    using FlowGraph = ffnx::graph::FlowGraph<std::string, std::string>;

    auto graph = std::make_shared<FlowGraph>();

    auto v0 = graph->add_vertex();
    auto v1 = graph->add_vertex();
    auto e = graph->add_edge(v0, v1);

    auto cluster = ffnx::cluster::Cluster<FlowGraph>::Builder(graph)
            .add_vertex(v0)
            .add_vertex(v1)
            .add_edge(e)
            .build();


    auto builder = ffnx::drplan::DRPlan<FlowGraph>::Builder(graph);

    auto plan = builder.build();

    ASSERT_EQ(0, plan.solution_tree().visit_child_nodes().size());
}

TEST(DRPlanTest, testBasicSolutionTree)
{
    using FlowGraph = ffnx::graph::FlowGraph<std::string, std::string>;

    auto graph = std::make_shared<FlowGraph>();

    auto v0 = graph->add_vertex();
    auto v1 = graph->add_vertex();
    auto e = graph->add_edge(v0, v1);

    auto cluster = ffnx::cluster::Cluster<FlowGraph>::Builder(graph)
            .add_vertex(v0)
            .add_vertex(v1)
            .add_edge(e)
            .build();

    auto builder = ffnx::drplan::DRPlan<FlowGraph>::Builder(graph);

    // remove v1 from the root cluster
    auto tree_root = builder.root().lock();

    auto sub_cluster = tree_root->value().get_vert_filtered_cluster([&v1](const auto &v){
        return v != v1;
    });

    builder.add_branch(builder.root(), *sub_cluster);

    auto plan = builder.build();

    ASSERT_EQ(1, plan.solution_tree().visit_child_nodes().size());
}

