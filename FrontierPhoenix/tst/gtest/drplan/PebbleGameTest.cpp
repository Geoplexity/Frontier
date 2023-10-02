#include <gtest/gtest.h>
#include <boost/graph/adjacency_list.hpp>
#include "ffnx/cluster/Cluster.h"
#include "ffnx/drplan/plans/canonical_top_down/PebbleGame2D.h"

TEST(PebbleGameTest, testCreate)
{
    using FlowGraph = ffnx::graph::FlowGraph<std::string, std::string>;

    auto graph = std::make_shared<FlowGraph>();

    auto v0 = graph->add_vertex();
    auto v1 = graph->add_vertex();
    auto e = graph->add_edge(v0, v1);

    std::shared_ptr<ffnx::cluster::Cluster<FlowGraph>> cluster = ffnx::cluster::Cluster<FlowGraph>::Builder(graph)
            .add_vertex(v0)
            .add_vertex(v1)
            .add_edge(e)
            .build();

    auto pebblegame = ffnx::pebblegame::PebbleGame2D<FlowGraph>(cluster);
}

TEST(PebbleGameTest, testRun)
{
    using FlowGraph = ffnx::graph::FlowGraph<std::string, std::string>;

    auto graph = std::make_shared<FlowGraph>();

    auto v0 = graph->add_vertex();
    auto v1 = graph->add_vertex();
    auto e = graph->add_edge(v0, v1);

    std::shared_ptr<ffnx::cluster::Cluster<FlowGraph>> cluster = ffnx::cluster::Cluster<FlowGraph>::Builder(graph)
            .add_vertex(v0)
            .add_vertex(v1)
            .add_edge(e)
            .build();

    auto game = ffnx::pebblegame::PebbleGame2D<FlowGraph>(cluster);
    game.run([](auto m){ return false; });
}