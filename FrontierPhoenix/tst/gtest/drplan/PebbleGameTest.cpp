#include <gtest/gtest.h>
#include <boost/graph/adjacency_list.hpp>
#include "cluster/Cluster.h"
#include "drplan/plans/pebble_game/PebbleGame2D.h"

TEST(PebbleGameTest, testCreate)
{
    using FlowGraph = ffnx::flowgraph::FlowGraph<std::string, std::string>;

    auto graph = std::make_shared<FlowGraph>();

    auto v0 = graph->add_vertex();
    auto v1 = graph->add_vertex();
    auto e = graph->add_edge(v0, v1);

    std::shared_ptr<ffnx::cluster::Cluster<std::string, std::string>> cluster = ffnx::cluster::Cluster<std::string, std::string>::Builder(graph)
            .add_vertex(v0)
            .add_vertex(v1)
            .add_edge(e)
            .build();

    auto pebblegame = ffnx::pebblegame::PebbleGame2D<std::string, std::string>(cluster);
}

TEST(PebbleGameTest, testRun)
{
    using FlowGraph = ffnx::flowgraph::FlowGraph<std::string, std::string>;

    auto graph = std::make_shared<FlowGraph>();

    auto v0 = graph->add_vertex();
    auto v1 = graph->add_vertex();
    auto e = graph->add_edge(v0, v1);

    std::shared_ptr<ffnx::cluster::Cluster<std::string, std::string>> cluster = ffnx::cluster::Cluster<std::string, std::string>::Builder(graph)
            .add_vertex(v0)
            .add_vertex(v1)
            .add_edge(e)
            .build();

    auto game = ffnx::pebblegame::PebbleGame2D<std::string, std::string>(cluster);
    auto result = game.run([](const auto &evt, const auto& t){});
}