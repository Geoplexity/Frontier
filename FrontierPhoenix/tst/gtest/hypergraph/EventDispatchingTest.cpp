#include <gtest/gtest.h>
#include "gmock/gmock.h"

#include <hypergraph/EventDispatching.h>
#include <hypergraph/sparse/Sparse.h>

using namespace ffnx::hypergraph;

using ::testing::AtLeast;
using ::testing::InSequence;
using ::testing::_;

class MockListener : public HyperGraphListener {
public:
    MOCK_METHOD(void, nodeAdded, (std::shared_ptr<HyperNode> node), (override));
    MOCK_METHOD(void, nodeRemoved, (std::shared_ptr<HyperNode> node), (override));

    MOCK_METHOD(void, edgeAdded, (std::shared_ptr<HyperEdge> node), (override));
    MOCK_METHOD(void, edgeRemoved, (std::shared_ptr<HyperEdge> node), (override));

    MOCK_METHOD(void, connected, (std::shared_ptr<HyperNode> node, std::shared_ptr<HyperEdge> edge), (override));
    MOCK_METHOD(void, disconnected, (std::shared_ptr<HyperNode> node, std::shared_ptr<HyperEdge> edge), (override));
};

TEST(EventDispatchingTests, testEvent)
{
    auto mhg = EventDispatchingHyperGraph(std::make_shared<SparseHyperGraph>());

    auto listener = std::make_shared<MockListener>();
    mhg.addListener(listener);

    EXPECT_CALL(*listener, nodeAdded).Times(1);

    auto node0 = mhg.addNode();
}


TEST(EventDispatchingTests, testEvent2)
{
    auto mhg = EventDispatchingHyperGraph(std::make_shared<SparseHyperGraph>());

    auto listener = std::make_shared<MockListener>();
    mhg.addListener(listener);

    {
        InSequence seq;

        EXPECT_CALL(*listener, nodeAdded).Times(1);
        EXPECT_CALL(*listener, nodeRemoved).Times(1);
    }

    auto node0 = mhg.addNode();
    mhg.removeNode(node0);
}

