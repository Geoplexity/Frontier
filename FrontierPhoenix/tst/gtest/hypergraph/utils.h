#ifndef FRONTIER_PHOENIX_TST_HYPERGRAPH_UTILS_H
#define FRONTIER_PHOENIX_TST_HYPERGRAPH_UTILS_H

#include "ffnx/hypergraph/HyperGraph.h"

using namespace ffnx::hypergraph;

inline int countEdges(const HyperGraph& graph) {
    int count = 0;

    graph.visitEdges([&count](auto e){
        count++;
    });

    return count;
}

inline int countNodes(const HyperGraph& graph) {
    int count = 0;

    graph.visitNodes([&count](auto e){
        count++;
    });

    return count;
}

inline int countNodes(const HyperGraph& hyperGraph, const std::weak_ptr<HyperEdge>& edge) {
    int count = 0;

    hyperGraph.visitEdgeNodes(edge, [&count](auto e){
        count++;
    });

    return count;
}


#endif