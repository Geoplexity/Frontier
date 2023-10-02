#include <gtest/gtest.h>
#include "gmock/gmock.h"

#include <ffnx/hypergraph/sparse/Sparse.h>
#include <ffnx/hypergraph/Annotator.h>

#include "./utils.h"

TEST(AnnotatorTests, testAssociateValues) {
    auto annotated = ffnx::hypergraph::AnnotatedHypergraph<int, bool>::create();

    auto nodeA = annotated.addNode(1);
    auto nodeB = annotated.addNode(2);

    ASSERT_EQ(countNodes(annotated.graph()), 2);

}