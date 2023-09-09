#ifndef FRONTIER_PHOENIX_FLOWGRAPH_BOOSTDEF_H
#define FRONTIER_PHOENIX_FLOWGRAPH_BOOSTDEF_H

#include <iostream>
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/copy.hpp>

namespace ffnx::flowgraph {

    using GVizAttrs = std::map<std::string, std::string>;

    using boost::listS;
    using boost::vecS;
    using boost::directedS;

    template <typename TVertexDataType, typename TEdgeDataType>
    using flow_graph_boost_def = boost::adjacency_list<
            vecS, // OutEdgeListS
            vecS, // VertexListS
            directedS, // directed/undirected

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


    template <typename TVertexDataType, typename TEdgeDataType>
    class FlowGraph : public flow_graph_boost_def<TVertexDataType, TEdgeDataType> {
    public:

        using traits = typename boost::graph_traits<flow_graph_boost_def<TVertexDataType, TEdgeDataType>>;
        using vdesc = typename traits::vertex_descriptor;
        using edesc = typename traits::edge_descriptor;

        std::unique_ptr<const FlowGraph<TVertexDataType, TEdgeDataType>> create_immutable_copy() const {
            auto result = std::make_unique<const FlowGraph<TVertexDataType, TEdgeDataType>>();

            boost::copy_graph(*this, *result);

            return result;
        }

        auto out_verts(vdesc & vert) {
            return boost::make_iterator_range(boost::adjacent_vertices(vert, *this));
        }

        auto edges() const {
            return boost::make_iterator_range(boost::edges(*this));
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

        std::pair<vdesc, vdesc> vertices_for_edge(edesc& edge) const {
            return std::make_pair(
                    boost::source(edge, *this),
                    boost::target(edge, *this));
        }
    };

}

#endif