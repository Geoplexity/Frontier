#include <gtest/gtest.h>

#include <hypergraph/Sparse.h>
#include "./utils.h"

using namespace ffnx::hypergraph;


TEST(SparseTests, testInit)
{
    auto shg = SparseHyperGraph();

    ASSERT_EQ(countNodes(shg), 0);
    ASSERT_EQ(countEdges(shg), 0);
}

TEST(SparseTests, testAddNode)
{
    auto shg = SparseHyperGraph();

    auto nodeA = shg.addNode();

    ASSERT_EQ(countNodes(shg), 1);
    ASSERT_EQ(countEdges(shg), 0);
}

TEST(SparseTests, testRemoveNode)
{
    auto shg = SparseHyperGraph();

    auto nodeA = shg.addNode();

    shg.removeNode(nodeA);

    ASSERT_EQ(countNodes(shg), 0);
    ASSERT_EQ(countEdges(shg), 0);
}

TEST(SparseTests, testDoubleRemoveNode)
{
    auto shg = SparseHyperGraph();

    auto nodeA = shg.addNode();

    shg.removeNode(nodeA);

    ASSERT_THROW(shg.removeNode(nodeA), std::runtime_error);
}

TEST(SparseTests, testAddEdge)
{
    auto shg = SparseHyperGraph();

    auto edgeA = shg.addEdge();

    ASSERT_EQ(countNodes(shg), 0);
    ASSERT_EQ(countEdges(shg), 1);
}


TEST(SparseTests, testRemoveEdge)
{
    auto shg = SparseHyperGraph();

    auto edgeA = shg.addEdge();

    shg.removeEdge(edgeA);

    ASSERT_EQ(countNodes(shg), 0);
    ASSERT_EQ(countEdges(shg), 0);
}

TEST(SparseTests, testDoubleRemoveEdge)
{
    auto shg = SparseHyperGraph();

    auto edgeA = shg.addEdge();

    shg.removeEdge(edgeA);

    ASSERT_THROW(shg.removeEdge(edgeA), std::runtime_error);
}



class DerivedHyperNode : public HyperNode {

};

class DerivedHyperEdge : public HyperEdge {

};

TEST(SparseTests, testDetectsOwnership)
{
    auto shg = SparseHyperGraph();

    auto nodeA = shg.addNode();
    auto nodeB = std::make_shared<DerivedHyperNode>();
    auto nodeC = std::make_shared<HyperNode>();

    auto edgeA = shg.addEdge();
    auto edgeB = std::make_shared<DerivedHyperEdge>();
    auto edgeC = std::make_shared<HyperEdge>();

    ASSERT_TRUE(shg.containsNode(nodeA));
    ASSERT_FALSE(shg.containsNode(nodeB));
    ASSERT_FALSE(shg.containsNode(nodeC));

    ASSERT_TRUE(shg.containsEdge(edgeA));
    ASSERT_FALSE(shg.containsEdge(edgeB));
    ASSERT_FALSE(shg.containsEdge(edgeC));
}
