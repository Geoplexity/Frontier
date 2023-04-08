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
}
