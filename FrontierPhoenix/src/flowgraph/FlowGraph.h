#ifndef FRONTIER_PHOENIX_FLOWGRAPH_BOOSTDEF_H
#define FRONTIER_PHOENIX_FLOWGRAPH_BOOSTDEF_H

#include <iostream>
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>

namespace ffnx::flowgraph {

    using GVizAttrs = std::map<std::string, std::string>;

    using boost::listS;
    using boost::vecS;
    using boost::directedS;

    template <typename TVertexDataType, typename TEdgeDataType>
    using FlowGraph = boost::adjacency_list<
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

}

#endif