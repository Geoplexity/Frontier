#include <gtest/gtest.h>
#include <boost/graph/adjacency_list.hpp>
#include "ffnx/graph/Graph.h"

TEST(DirectedGraphTests, testCreate)
{
    using DirGraph = ffnx::graph::DirectedGraph<std::string, std::string>;

    DirGraph graph;

    auto v0 = graph.add_vertex();
    auto v1 = graph.add_vertex();
    auto v2 = graph.add_vertex();
    auto v3 = graph.add_vertex();
    auto v4 = graph.add_vertex();
    auto v5 = graph.add_vertex();
    auto v6 = graph.add_vertex();
    auto v7 = graph.add_vertex();

    graph.add_edge(v0, v1);
    graph.add_edge(v0, v2);

    graph.add_edge(v2, v3);
    graph.add_edge(v3, v4);
    graph.add_edge(v3, v5);

    graph.add_edge(v5, v6);
    graph.add_edge(v4, v6);

    graph.add_edge(v6, v7);

    auto r0 = graph.compute_reach(v0);
    auto r1 = graph.compute_reach(v1);
    auto r2 = graph.compute_reach(v2);
    auto r3 = graph.compute_reach(v3);
    auto r4 = graph.compute_reach(v4);
    auto r5 = graph.compute_reach(v5);
    auto r6 = graph.compute_reach(v6);
    auto r7 = graph.compute_reach(v7);

    std::set<DirGraph::vdesc> expected_7{};
    ASSERT_EQ(*r7, expected_7);

    std::set<DirGraph::vdesc> expected_6{ v7 };
    ASSERT_EQ(*r6, expected_6);

    std::set<DirGraph::vdesc> expected_5{ v6, v7 };
    ASSERT_EQ(*r5, expected_5);

    std::set<DirGraph::vdesc> expected_4{ v6, v7 };
    ASSERT_EQ(*r4, expected_4);

    std::set<DirGraph::vdesc> expected_3{ v4, v5, v6, v7 };
    ASSERT_EQ(*r3, expected_3);

    std::set<DirGraph::vdesc> expected_2{ v3, v4, v5, v6, v7 };
    ASSERT_EQ(*r2, expected_2);

    std::set<DirGraph::vdesc> expected_1{};
    ASSERT_EQ(*r1, expected_1);

    std::set<DirGraph::vdesc> expected_0{ v1, v2, v3, v4, v5, v6, v7 };
    ASSERT_EQ(*r0, expected_0);

}
