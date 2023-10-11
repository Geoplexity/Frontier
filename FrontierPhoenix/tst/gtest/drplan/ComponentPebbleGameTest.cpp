#include <gtest/gtest.h>
#include <boost/graph/adjacency_list.hpp>
#include "ffnx/cluster/Cluster.h"
#include "ffnx/drplan/plans/canonical_top_down/PebbleGame2D.h"
#include "ffnx/drplan/plans/canonical_top_down/ComponentDetector.h"

TEST(ComponentPebbleGameTest, testCreate)
{
    using Graph = ffnx::graph::UndirectedGraph<std::string, std::string>;

    auto graph = std::make_shared<Graph>();

    auto v0 = graph->add_vertex();
    auto v1 = graph->add_vertex();
    auto e = graph->add_edge(v0, v1);

    std::shared_ptr<ffnx::cluster::Cluster<Graph>> cluster = ffnx::cluster::Cluster<Graph>::Builder(graph)
            .add_vertex(v0)
            .add_vertex(v1)
            .add_edge(e)
            .build();

    auto pebblegame = std::make_shared<ffnx::pebblegame::PebbleGame2D<Graph>>(cluster);

    ffnx::pebblegame::ComponentDetector<Graph> cd(pebblegame);
    pebblegame->run([&cd](auto m){ cd.process_move(m); });
}
