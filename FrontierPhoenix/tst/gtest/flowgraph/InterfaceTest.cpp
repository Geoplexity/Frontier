#include <gtest/gtest.h>
#include <boost/graph/adjacency_list.hpp>

#include "flowgraph/Interface.h"
#include "flowgraph/commands/AddVertex.h"
#include "flowgraph/commands/AddEdge.h"

using namespace ffnx::flowgraph;

TEST(InterfaceTests, testCreate)
{
    using Interface = FlowGraphInterface<std::string, std::string>;
    using AddVertex = commands::AddVertexCommand<std::string, std::string>;
    using AddEdge = commands::AddEdgeCommand<std::string, std::string>;

    Interface interface;

    auto v0 = interface.applyCommand(std::make_shared<AddVertex>())->getVertex();
    auto v1 = interface.applyCommand(std::make_shared<AddVertex>())->getVertex();

    auto e0 = interface.applyCommand(std::make_shared<AddEdge>(v0, v1))->getEdge();
}
