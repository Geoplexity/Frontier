#include <gtest/gtest.h>
#include <boost/graph/adjacency_list.hpp>
#include "ffnx/cluster/Cluster.h"
#include "ffnx/drplan/plans/canonical_top_down/PebbleGame2D.h"
#include "ffnx/drplan/plans/canonical_top_down/ComponentDetector.h"

TEST(ComponentIPebbleGameTest, testCreate)
{
    using Graph = ffnx::graph::UndirectedGraph<std::string, std::string>;

    auto graph = std::make_shared<Graph>();

    auto v0 = graph->add_vertex();
    auto v1 = graph->add_vertex();
    auto v2 = graph->add_vertex();
    auto v3 = graph->add_vertex();
    auto e0 = graph->add_edge(v0, v1);
    auto e1 = graph->add_edge(v0, v1);

    std::shared_ptr<ffnx::cluster::Cluster<Graph>> cluster = ffnx::cluster::Cluster<Graph>::Builder::of_graph(graph);

    auto pebblegame = std::make_shared<ffnx::pebblegame::PebbleGame2D<Graph>>(cluster);

    ffnx::pebblegame::ComponentDetectorI<Graph> cd(pebblegame);
    pebblegame->run([&cd](auto m){
        cd.process_move(m);
    });
}


TEST(ComponentIIPebbleGameTest, testCreate)
{
    using Graph = ffnx::graph::UndirectedGraph<std::string, std::string>;

    auto graph = std::make_shared<Graph>();

    // create the basic "triangular prism" graph

    auto v0 = graph->add_vertex();
    auto v1 = graph->add_vertex();
    auto v2 = graph->add_vertex();

    auto v3 = graph->add_vertex();
    auto v4 = graph->add_vertex();
    auto v5 = graph->add_vertex();

    graph->add_edge(v0, v1);
    graph->add_edge(v1, v2);
    graph->add_edge(v2, v3);

    graph->add_edge(v3, v4);
    graph->add_edge(v4, v5);
    graph->add_edge(v5, v3);

    graph->add_edge(v0, v3);
    graph->add_edge(v1, v4);
    //graph->add_edge(v2, v5);

    std::shared_ptr<ffnx::cluster::Cluster<Graph>> cluster = ffnx::cluster::Cluster<Graph>::Builder::of_graph(graph);

    auto pebblegame = std::make_shared<ffnx::pebblegame::PebbleGame2D<Graph>>(cluster);

    ffnx::pebblegame::ComponentDetectorII<Graph> cd(pebblegame);
    pebblegame->run([&cd](auto m){
        cd.process_move(m);
    });
}
