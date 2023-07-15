#include <gtest/gtest.h>
#include <boost/graph/adjacency_list.hpp>
#include "flowgraph/FlowGraph.h"

TEST(FlowGraphTests, testCreate)
{
    using FlowGraph = ffnx::flowgraph::FlowGraph<std::string, std::string>;

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

TEST(FlowGraphTests, testAdjacentVertHelper)
{
    using FlowGraph = ffnx::flowgraph::FlowGraph<std::string, std::string>;

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
