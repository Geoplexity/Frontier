#ifndef FRONTIER_PHOENIX_CLUSTER_CLUSTER_H
#define FRONTIER_PHOENIX_CLUSTER_CLUSTER_H

#include "flowgraph/FlowGraph.h"
#include <stdexcept>

namespace ffnx::cluster {

    /**
     * Identifies a subset of edges/verts of the flow graph.
     */
    template <typename TFlowVertType, typename TFlowEdgeType>
    class Cluster {
    private:
        using graph_ptr = std::weak_ptr<flowgraph::FlowGraph<TFlowVertType, TFlowEdgeType>>;

        graph_ptr graph;
        std::set<TFlowVertType> _vertices;
        std::set<TFlowEdgeType> _edges;

    public:
        Cluster(graph_ptr graph, std::set<TFlowVertType>& vertices, std::set<TFlowEdgeType>& edges) :
            graph(graph), _vertices(std::move(vertices)), _edges(std::move(edges)) {

        }

        /**
         * Returns a new cluster where the edges/verts are filtered according
         * to the supplied predicate.
         * @param vert_filter function returns true if a vert should be preserved
         * @param edge_filter function returns true if an edge should be preserved
         * @return
         */
        std::shared_ptr<Cluster<TFlowVertType, TFlowEdgeType>> get_filtered_cluster(
                const std::function<bool(const TFlowVertType&)>& vert_filter,
                const std::function<bool(const TFlowEdgeType&)>& edge_filter) {

            auto builder = Cluster<TFlowVertType, TFlowEdgeType>::Builder(graph);

            for (const auto& v : _vertices) {
                if (vert_filter(v)) {
                    builder.add_vertex(v);
                }
            }

            for (const auto& e : _edges) {
                if (edge_filter(e)) {
                    builder.add_edge(e);
                }
            }

            return builder.build();
        }

        std::shared_ptr<Cluster<TFlowVertType, TFlowEdgeType>> get_edge_filtered_cluster(
                const std::function<bool(const TFlowEdgeType&)>& edge_filter) {
            return get_filtered_cluster(
                    [](const TFlowVertType& v){ return true; },
                    edge_filter);
        }

        std::shared_ptr<Cluster<TFlowVertType, TFlowEdgeType>> get_vert_filtered_cluster(
                const std::function<bool(const TFlowVertType&)>& vert_filter) {
            return get_filtered_cluster(
                    vert_filter,
                    [](const TFlowEdgeType& v){ return true; });
        }

        const std::set<TFlowVertType>& vertices() {
            return _vertices;
        }

        const std::set<TFlowEdgeType>& edges() {
            return _edges;
        }

        class Builder {
        private:
            using graph_ptr = std::weak_ptr<flowgraph::FlowGraph<TFlowVertType, TFlowEdgeType>>;

            graph_ptr graph;
            std::set<TFlowVertType> vertices;
            std::set<TFlowEdgeType> edges;

            bool has_built = false;

        public:
            Builder(const graph_ptr& graph) : graph(graph) {

            }

            Builder& add_vertex(const TFlowVertType& vertex) {
                assert_not_built();
                if (vertices.contains(vertex)) {
                    throw std::runtime_error("Attempt to add duplicate vertex to cluster. This is likely an error.");
                }

                vertices.insert(vertex);
            }

            Builder& add_edge(const TFlowEdgeType& edge) {
                assert_not_built();
                if (edges.contains(edge)) {
                    throw std::runtime_error("Attempt to add duplicate edge to cluster. This is likely an error.");
                }

                edges.insert(edge);
            }

            std::shared_ptr<Cluster<TFlowVertType, TFlowEdgeType>> build() {
                assert_not_built();
                has_built = true;
                return std::make_shared<Cluster<TFlowVertType, TFlowEdgeType>>(graph, vertices, edges);
            }

        private:
            void assert_not_built() {
                if (has_built) {
                    throw std::runtime_error("Build already called");
                }
            }
        };
    };

}


#endif