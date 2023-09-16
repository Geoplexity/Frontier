#ifndef FRONTIER_PHOENIX_CLUSTER_CLUSTER_H
#define FRONTIER_PHOENIX_CLUSTER_CLUSTER_H

#include "graph/Graph.h"
#include <stdexcept>

namespace ffnx::cluster {

    /**
     * Identifies a subset of edges/verts of the flow graph.
     */
    template <typename TGraph>
    class Cluster {
    private:
        using VDesc = typename TGraph::vertex_descriptor;
        using EDesc = typename TGraph::edge_descriptor;

        using graph_ptr = std::weak_ptr<const TGraph>;

        graph_ptr _graph;
        std::set<VDesc> _vertices;
        std::set<EDesc> _edges;

    public:
        Cluster(graph_ptr graph, std::set<VDesc>& vertices, std::set<EDesc>& edges) :
            _graph(graph), _vertices(std::move(vertices)), _edges(std::move(edges)) {

        }

        /**
         * Copy constructor
         * @param other
         */
        Cluster(const Cluster<TGraph>& other) :
            _graph(other._graph), _vertices(other._vertices), _edges(other._edges) {

        }

        graph_ptr graph() const {
            return _graph;
        }

        /**
         * Returns a new cluster where the edges/verts are filtered according
         * to the supplied predicate.
         * @param vert_filter function returns true if a vert should be preserved
         * @param edge_filter function returns true if an edge should be preserved
         * @return
         */
        std::shared_ptr<Cluster<TGraph>> get_filtered_cluster(
                const std::function<bool(const VDesc&)>& vert_filter,
                const std::function<bool(const EDesc&)>& edge_filter) const {

            auto builder = Cluster<TGraph>::Builder(_graph);

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

        std::shared_ptr<Cluster<TGraph>> get_edge_filtered_cluster(
                const std::function<bool(const EDesc&)>& edge_filter) const {
            return get_filtered_cluster(
                    [](const VDesc& v){ return true; },
                    edge_filter);
        }

        std::shared_ptr<Cluster<TGraph>> get_vert_filtered_cluster(
                const std::function<bool(const VDesc&)>& vert_filter) const {
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

        bool shares_graph(const Cluster<TGraph>& other) const {
            return _graph.lock() == other._graph.lock();
        }

        void assert_shares_graph(const Cluster<TGraph>& other) const {
            if (!shares_graph(other)) {
                throw std::runtime_error("The supplied cluster is based off a different root graph.");
            }
        }

        /**
         * @return true if the set of vertices of this cluster is a maximal subgraph (has precisely one fewer element)
         * of other. Edges are not considered by the comparison.
         */
        bool is_vertex_maximal_subgraph_of(const Cluster<TGraph>& other) const {
            return other.includes(*this) && other._vertices.size() == this->_vertices.size() - 1;
        }

        /**
         * @return true if the intersection of this cluster with other contains exactly one vertex.
         */
        bool intersects_trivially(const Cluster<TGraph>& other) const {
            return intersection(other).vertices().size() == 1;
        }

        /**
         * @return a new cluster which is the intersection (boolean AND) of this and other. Both vertices and edges
         * are considered for this.
         */
        std::unique_ptr<Cluster<TGraph>> intersection(const Cluster<TGraph>& other) const {
            assert_shares_graph(other);

            auto result = Cluster<TGraph>::Builder(_graph);

            for (const auto &v : std::set_intersection(
                    this->_vertices.begin(), this->_vertices.end(),
                    other._vertices.begin(), other._vertices.end())) {
                result.add_vertex(v);
            }

            for (const auto &e : std::set_intersection(
                    this->_edges.begin(), this->_edges.end(),
                    other._edges.begin(), other._edges.end())) {
                result.add_edge(e);
            }

            return result.build();
        }

        /**
         * @param other
         * @return true if the set of vertices and set of edges in this cluster exactly match that of other
         */
        bool is_equivalent(const Cluster<TGraph>& other) const {
            assert_shares_graph(other);

            return _vertices == other._vertices && _edges == other._edges;
        }

        /**
         * Equivalent to std::includes for both vertices and edges. Graphs must match.
         */
        bool includes(const Cluster<TGraph> other) const {
            assert_shares_graph(other);

            return std::includes(_vertices.begin(), _vertices.end(), other._vertices.begin(), other._vertices.end()) &&
                std::includes(_edges.begin(), _edges.end(), other._edges.begin(), other._edges.end());
        }

        bool includes_vertex(const VDesc& vertex) const {
            return includes(*(Builder(_graph).add_vertex(vertex).build()));
        }

        bool includes_edge(const EDesc& edge) const {
            return includes(*(Builder(_graph).add_edge(edge).build()));
        }


        class Builder {
        private:
            using graph_ptr = std::weak_ptr<const TGraph>;

            graph_ptr graph;
            std::set<VDesc> vertices;
            std::set<EDesc> edges;

            bool has_built = false;

        public:
            explicit Builder(graph_ptr graph) : graph(graph), vertices(), edges() {

            }

            static std::shared_ptr<Cluster<TGraph>> of_graph(graph_ptr graph) {
                Cluster<TGraph>::Builder builder(graph);
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

            std::unique_ptr<Cluster<TGraph>> build() {
                assert_not_built();
                has_built = true;
                return std::make_unique<Cluster<TGraph>>(graph, vertices, edges);
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