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
        using FlowGraph = flowgraph::FlowGraph<TFlowVertType, TFlowEdgeType>;
        using VDesc = typename FlowGraph::vertex_descriptor;
        using EDesc = typename FlowGraph::edge_descriptor;

        using graph_ptr = std::weak_ptr<flowgraph::FlowGraph<TFlowVertType, TFlowEdgeType>>;


        graph_ptr graph;
        std::set<VDesc> _vertices;
        std::set<EDesc> _edges;

    public:
        Cluster(graph_ptr graph, std::set<VDesc>& vertices, std::set<EDesc>& edges) :
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
                const std::function<bool(const VDesc&)>& vert_filter,
                const std::function<bool(const EDesc&)>& edge_filter) {

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
                const std::function<bool(const EDesc&)>& edge_filter) {
            return get_filtered_cluster(
                    [](const VDesc& v){ return true; },
                    edge_filter);
        }

        std::shared_ptr<Cluster<TFlowVertType, TFlowEdgeType>> get_vert_filtered_cluster(
                const std::function<bool(const VDesc&)>& vert_filter) {
            return get_filtered_cluster(
                    vert_filter,
                    [](const EDesc& v){ return true; });
        }

        const std::set<VDesc>& vertices() const {
            return _vertices;
        }

        const std::set<EDesc>& edges() const {
            return _edges;
        }

        bool shares_graph(const Cluster<TFlowVertType, TFlowEdgeType>& other) {
            return graph.lock() == other.graph.lock();
        }

        /**
         * @param other
         * @return true if the set of vertices and set of edges in this cluster exactly match that of other
         */
        bool is_equivalent(const Cluster<TFlowVertType, TFlowEdgeType>& other) {
            return shares_graph(other) && _vertices == other._vertices && _edges == other._edges;
        }

        /**
         * Equivalent to std::includes for both vertices and edges. Graphs must match.
         */
        bool includes(const Cluster<TFlowVertType, TFlowEdgeType> other) {
            return shares_graph(other) &&
                std::includes(_vertices.begin(), _vertices.end(), other._vertices.begin(), other._vertices.end()) &&
                std::includes(_edges.begin(), _edges.end(), other._edges.begin(), other._edges.end());
        }

        class Builder {
        private:
            using graph_ptr = std::weak_ptr<flowgraph::FlowGraph<TFlowVertType, TFlowEdgeType>>;

            graph_ptr graph;
            std::set<VDesc> vertices;
            std::set<EDesc> edges;

            bool has_built = false;

        public:
            explicit Builder(const graph_ptr& graph) : graph(graph), vertices(), edges() {

            }

            static std::shared_ptr<Cluster<TFlowVertType, TFlowEdgeType>> of_graph(const graph_ptr& graph) {
                Cluster<TFlowVertType, TFlowEdgeType>::Builder builder(graph);
                for (const auto& v: graph.lock()->vertices()) {
                    builder.add_vertex(v);
                }

                for (const auto& e: graph.lock()->edges()) {
                    builder.add_edge(e);
                }

                return builder.build();
            }

            Builder& add_vertex(const VDesc& vertex) {
                assert_not_built();
                if (vertices.contains(vertex)) {
                    throw std::runtime_error("Attempt to add duplicate vertex to cluster. This is likely an error.");
                }

                vertices.insert(vertex);

                return *this;
            }

            Builder& add_edge(const EDesc& edge) {
                assert_not_built();
                if (edges.contains(edge)) {
                    throw std::runtime_error("Attempt to add duplicate edge to cluster. This is likely an error.");
                }

                edges.insert(edge);

                return *this;
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