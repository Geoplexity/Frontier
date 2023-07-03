#ifndef FRONTIER_PHOENIX_CLUSTER_CLUSTER_H
#define FRONTIER_PHOENIX_CLUSTER_CLUSTER_H

#include "flowgraph/FlowGraph.h"

namespace ffnx::cluster {

    /**
     * Identifies a subset of edges/verts of the flow graph.
     */
    template <typename TFlowVertType, typename TFlowEdgeType>
    class Cluster {
    private:
        std::set<TFlowVertType> vertices;
        std::set<TFlowEdgeType> edges;

    public:
        Cluster() : vertices(), edges() {

        }

        Cluster(const std::set<TFlowVertType>& vertices, const std::set<TFlowEdgeType>& edges) :
            vertices(vertices), edges(edges) {

        }

        void addVertex(const TFlowVertType& vertex) {
            if (vertices.contains(vertex)) {
                throw std::runtime_error("Attempt to add duplicate vertex to cluster. This is likely an error.");
            }

            vertices.insert(vertex);
        }

        void addEdge(const TFlowEdgeType& edge) {
            if (edges.contains(edge)) {
                throw std::runtime_error("Attempt to add duplicate edge to cluster. This is likely an error.");
            }

            edges.insert(edge);
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