#ifndef FRONTIER_PHOENIX_CLUSTER_CLUSTER_H
#define FRONTIER_PHOENIX_CLUSTER_CLUSTER_H

#include "flowgraph/FlowGraph.h"

namespace ffnx::cluster {

    /**
     * Identifies a subset of hyperedges/verts of the
     */
    template <typename TFlowVertType, typename TFlowEdgeType>
    class Cluster {
    private:
        std::set<TFlowVertType> vertices;
        std::set<TFlowEdgeType> edges;

    public:
        Cluster(const std::set<TFlowVertType>& vertices, const std::set<TFlowEdgeType>& edges) :
            vertices(vertices), edges(edges) {

        }

        const std::set<TFlowVertType>& getVertices() {
            return vertices;
        }

        const std::set<TFlowEdgeType>& getEdges() {
            return edges;
        }

    };

}


#endif