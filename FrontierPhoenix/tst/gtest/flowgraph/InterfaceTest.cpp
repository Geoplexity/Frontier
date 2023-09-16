#include <gtest/gtest.h>
#include <boost/graph/adjacency_list.hpp>

#include "graph/Interface.h"
#include "graph/commands/AddVertex.h"
#include "graph/commands/AddEdge.h"

using namespace ffnx::graph;

TEST(InterfaceTests, testCreate)
{
    using FlowGraph = ffnx::graph::FlowGraph<std::string, std::string>;
    using Interface = FlowGraphInterface<std::string, std::string>;
    using AddVertex = commands::AddVertexCommand<FlowGraph>;
    using AddEdge = commands::AddEdgeCommand<FlowGraph>;

    Interface interface;

    auto v0 = interface.applyCommand(std::make_shared<AddVertex>())->getVertex();
    auto v1 = interface.applyCommand(std::make_shared<AddVertex>())->getVertex();

    auto e0 = interface.applyCommand(std::make_shared<AddEdge>(v0, v1))->getEdge();
}
