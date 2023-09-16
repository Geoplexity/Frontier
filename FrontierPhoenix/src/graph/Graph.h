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

        std::unique_ptr<const Graph<TVertexDataType, TEdgeDataType, directedType>> create_immutable_copy() const {
            auto result = std::make_unique<const Graph<TVertexDataType, TEdgeDataType, directedType>>();

            boost::copy_graph(*this, *result);

            return result;
        }

        auto in_verts(vdesc & vert) const {
            return boost::make_iterator_range(boost::inv_adjacent_vertices(vert, *this));
        }

        auto out_verts(vdesc & vert) const {
            return boost::make_iterator_range(boost::adjacent_vertices(vert, *this));
        }

        auto in_edges(vdesc & vert) const {
            return boost::make_iterator_range(boost::in_edges(vert, *this));
        }

        auto out_edges(vdesc & vert) const {
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

        edesc add_edge(vdesc& v0, vdesc& v1) {
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
    class FlowGraph : public Graph<TVertexDataType, TEdgeDataType, bidirectionalS> {
    public:
        using directionality = boost::bidirectionalS;

    };
}

#endif