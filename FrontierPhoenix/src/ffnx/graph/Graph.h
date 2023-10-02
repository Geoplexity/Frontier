#ifndef FRONTIER_PHOENIX_GRAPH_BOOSTDEF_H
#define FRONTIER_PHOENIX_GRAPH_BOOSTDEF_H

#include <iostream>
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/copy.hpp>

namespace ffnx::graph {

    using GVizAttrs = std::map<std::string, std::string>;

    using boost::listS;
    using boost::vecS;

    using boost::directedS;
    using boost::bidirectionalS;
    using boost::undirectedS;

    template <typename TVertexDataType, typename TEdgeDataType, typename directedType>
    using graph_boost_def = boost::adjacency_list<
            vecS, // OutEdgeListS
            vecS, // VertexListS
            directedType, // directed/undirected

            // vertex properties
            boost::property<
                boost::vertex_index_t, int, // vertex index required for graphviz
            boost::property<
                boost::vertex_attribute_t, GVizAttrs,
            TVertexDataType>>,

            // edge properties
            boost::property<
                boost::edge_index_t, int, // edge index required for graphviz (?)
            boost::property<
                boost::edge_weight_t, int,
            boost::property<
                boost::edge_attribute_t, GVizAttrs,
            TEdgeDataType>>>,

            // graph properties
            boost::property<
                boost::graph_name_t, std::string,
            boost::property<
                boost::graph_graph_attribute_t, GVizAttrs,
            boost::property<
                boost::graph_vertex_attribute_t, GVizAttrs,
            boost::property<
                boost::graph_edge_attribute_t, GVizAttrs>>>>
    >;


    template <typename TVertexDataType, typename TEdgeDataType, typename directedType>
    class Graph : public graph_boost_def<TVertexDataType, TEdgeDataType, directedType> {
    public:

        using vertex_data_type = TVertexDataType;
        using edge_data_type = TEdgeDataType;
        using traits = typename boost::graph_traits<graph_boost_def<TVertexDataType, TEdgeDataType, directedType>>;
        using vdesc = typename traits::vertex_descriptor;
        using edesc = typename traits::edge_descriptor;

        template<typename new_directionality>
        using with_different_directionality = Graph<TVertexDataType, TEdgeDataType, new_directionality>;

        std::unique_ptr<const Graph<TVertexDataType, TEdgeDataType, directedType>> create_immutable_copy() const {
            auto result = std::make_unique<const Graph<TVertexDataType, TEdgeDataType, directedType>>();

            boost::copy_graph(*this, *result);

            return result;
        }

        auto in_verts(const vdesc & vert) const {
            return boost::make_iterator_range(boost::inv_adjacent_vertices(vert, *this));
        }

        auto out_verts(const vdesc & vert) const {
            return boost::make_iterator_range(boost::adjacent_vertices(vert, *this));
        }

        auto in_edges(const vdesc & vert) const {
            return boost::make_iterator_range(boost::in_edges(vert, *this));
        }

        auto out_edges(const vdesc & vert) const {
            return boost::make_iterator_range(boost::out_edges(vert, *this));
        }

        auto edges() const {
            return boost::make_iterator_range(boost::edges(*this));
        }

        auto edge(const vdesc& v0, const vdesc& v1) const {
            auto result = boost::edge(v0, v1, *this);

            if (!result.second) {
                throw std::runtime_error("The specified edge does not exist. "
                                         "Use has_edge to determine presence");
            }

            return result.first;
        }

        bool has_edge(const vdesc& v0, const vdesc& v1) const {
            return boost::edge(v0, v1, *this).second;
        }

        vdesc add_vertex() {
            return boost::add_vertex(*this);
        }

        edesc add_edge(const vdesc& v0, const vdesc& v1) {
            auto result = boost::add_edge(v0, v1, *this);

            auto is_successful = std::get<1>(result);

            if (!is_successful) {
                throw std::runtime_error("Add edge failed.");
            }

            return std::get<0>(result);
        }

        auto vertices() const {
            return boost::make_iterator_range(boost::vertices(*this));
        }

        std::pair<vdesc, vdesc> vertices_for_edge(const edesc& edge) const {
            vdesc source = boost::source(edge, *this);
            vdesc target = boost::target(edge, *this);
            return std::make_pair(source, target);
        }
    };

    template <typename TVertexDataType, typename TEdgeDataType>
    class UndirectedGraph : public Graph<TVertexDataType, TEdgeDataType, undirectedS> {
    public:
        using directionality = boost::undirectedS;

    };

    template <typename TVertexDataType, typename TEdgeDataType>
    class DirectedGraph : public Graph<TVertexDataType, TEdgeDataType, directedS> {
    public:
        using directionality = boost::directedS;

        using vdesc = typename Graph<TVertexDataType, TEdgeDataType, directedS>::vdesc;
        using edesc = typename Graph<TVertexDataType, TEdgeDataType, directedS>::edesc;

        std::unique_ptr<std::set<vdesc>> compute_reach(const vdesc &v) {
            auto result = std::make_unique<std::set<vdesc>>();

            std::stack<vdesc> next_verts;
            next_verts.push(v);

            while (!next_verts.empty()) {
                auto to_check = next_verts.top();
                next_verts.pop();

                result->insert(to_check);
                compute_direct_reach(*result, next_verts, to_check);
            }

            result->erase(v);

            return std::move(result);
        }

    private:
        void compute_direct_reach(const std::set<vdesc> &visited_verts,
                                  std::stack<vdesc>& to_explore,
                                  const vdesc &v_from) {
            for (const auto &e : this->out_edges(v_from)) {
                auto next_v = this->vertices_for_edge(e).second;

                if (!visited_verts.contains(next_v)) {
                    to_explore.push(next_v);
                }
            }
        }
    };

    template <typename TVertexDataType, typename TEdgeDataType>
    class BidirectionalGraph : public Graph<TVertexDataType, TEdgeDataType, bidirectionalS> {
    public:
        using directionality = boost::bidirectionalS;

    };

    template <typename TVertexDataType, typename TEdgeDataType>
    class FlowGraph : public Graph<TVertexDataType, TEdgeDataType, bidirectionalS> {
    public:
        using directionality = boost::bidirectionalS;

    };
}

#endif