#include <gtest/gtest.h>
#include <boost/graph/adjacency_list.hpp>
#include "ffnx/graph/Graph.h"

TEST(FlowGraphTests, testCreate)
{
    using FlowGraph = ffnx::graph::FlowGraph<std::string, std::string>;

    FlowGraph graph;

    auto v0 = boost::add_vertex(graph);
    auto v1 = boost::add_vertex(graph);

    auto e = boost::add_edge(v0, v1, graph);

    auto vert_it = graph.vertices();
    std::vector<FlowGraph::vertex_descriptor> verts(vert_it.begin(), vert_it.end());

    auto edge_it = graph.edges();
    std::vector<FlowGraph::edge_descriptor> edges(edge_it.begin(), edge_it.end());

    ASSERT_EQ(2, verts.size());
    ASSERT_EQ(1, edges.size());
}

TEST(FlowGraphTests, testCreateViaInstanceMethods)
{
    using FlowGraph = ffnx::graph::FlowGraph<std::string, std::string>;

    FlowGraph graph;

    auto v0 = graph.add_vertex();
    auto v1 = graph.add_vertex();

    auto e = graph.add_edge(v0, v1);

    auto vert_it = graph.vertices();
    std::vector<FlowGraph::vertex_descriptor> verts(vert_it.begin(), vert_it.end());

    auto edge_it = graph.edges();
    std::vector<FlowGraph::edge_descriptor> edges(edge_it.begin(), edge_it.end());

    ASSERT_EQ(2, verts.size());
    ASSERT_EQ(1, edges.size());
}

TEST(FlowGraphTests, testAdjacentOutVertHelper)
{
    using FlowGraph = ffnx::graph::FlowGraph<std::string, std::string>;

    FlowGraph graph;

    auto v0 = boost::add_vertex(graph);
    auto v1 = boost::add_vertex(graph);
    auto v2 = boost::add_vertex(graph);
    auto v3 = boost::add_vertex(graph);

    auto e_v0_v1 = boost::add_edge(v0, v1, graph);

    auto e_v1_v2 = boost::add_edge(v1, v2, graph);
    auto e_v1_v3 = boost::add_edge(v1, v3, graph);

    auto out_vit = graph.out_verts(v0);
    ASSERT_EQ(1, std::distance(out_vit.begin(), out_vit.end()));

    out_vit = graph.out_verts(v1);
    ASSERT_EQ(2, std::distance(out_vit.begin(), out_vit.end()));

    out_vit = graph.out_verts(v2);
    ASSERT_EQ(0, std::distance(out_vit.begin(), out_vit.end()));

    out_vit = graph.out_verts(v3);
    ASSERT_EQ(0, std::distance(out_vit.begin(), out_vit.end()));
}


TEST(FlowGraphTests, testAdjacentInVertHelper)
{
    using FlowGraph = ffnx::graph::FlowGraph<std::string, std::string>;

    FlowGraph graph;

    auto v0 = boost::add_vertex(graph);
    auto v1 = boost::add_vertex(graph);
    auto v2 = boost::add_vertex(graph);
    auto v3 = boost::add_vertex(graph);

    auto e_v0_v1 = boost::add_edge(v0, v1, graph);

    auto e_v1_v2 = boost::add_edge(v1, v2, graph);
    auto e_v1_v3 = boost::add_edge(v1, v3, graph);

    auto in_vit = graph.in_verts(v0);
    ASSERT_EQ(0, std::distance(in_vit.begin(), in_vit.end()));

    in_vit = graph.in_verts(v1);
    ASSERT_EQ(1, std::distance(in_vit.begin(), in_vit.end()));

    in_vit = graph.in_verts(v2);
    ASSERT_EQ(1, std::distance(in_vit.begin(), in_vit.end()));

    in_vit = graph.in_verts(v3);
    ASSERT_EQ(1, std::distance(in_vit.begin(), in_vit.end()));
}

TEST(FlowGraphTests, testEdgeVertsHelper)
{
    using FlowGraph = ffnx::graph::FlowGraph<std::string, std::string>;

    FlowGraph graph;

    auto v0 = boost::add_vertex(graph);
    auto v1 = boost::add_vertex(graph);
    auto v2 = boost::add_vertex(graph);
    auto v3 = boost::add_vertex(graph);

    auto e_v0_v1 = boost::add_edge(v0, v1, graph);
    auto e_v1_v2 = boost::add_edge(v1, v2, graph);
    auto e_v1_v3 = boost::add_edge(v1, v3, graph);
    auto e_v3_v0 = boost::add_edge(v3, v0, graph);

    auto expected_v0_v1 = graph.vertices_for_edge(e_v0_v1.first);
    ASSERT_EQ(expected_v0_v1.first, v0);
    ASSERT_EQ(expected_v0_v1.second, v1);

    auto expected_v1_v2 = graph.vertices_for_edge(e_v1_v2.first);
    ASSERT_EQ(expected_v1_v2.first, v1);
    ASSERT_EQ(expected_v1_v2.second, v2);

    auto expected_v1_v3 = graph.vertices_for_edge(e_v1_v3.first);
    ASSERT_EQ(expected_v1_v3.first, v1);
    ASSERT_EQ(expected_v1_v3.second, v3);

    auto expected_v3_v0 = graph.vertices_for_edge(e_v3_v0.first);
    ASSERT_EQ(expected_v3_v0.first, v3);
    ASSERT_EQ(expected_v3_v0.second, v0);
}
