#ifndef FRONTIER_PHOENIX_GRAPH_H
#define FRONTIER_PHOENIX_GRAPH_H

#include <iostream>
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>

namespace ffnx::entities {

    /**
     * Exists to map between graph input/output and arbitrary data.
     */
    struct MappingToken {

    };

    struct VertexData {
        std::string label;
    };

    struct EdgeData {
        std::string label;
    };

    using GVizAttrs = std::map<std::string, std::string>;

    using boost::listS;
    using boost::vecS;
    using boost::directedS;


    typedef boost::adjacency_list<
            vecS, // OutEdgeListS
            vecS, // VertexListS
            directedS, // directed/undirected

            // vertex properties
            boost::property<
                    boost::vertex_index_t, int, // vertex index required for graphviz
                    boost::property<
                            boost::vertex_attribute_t, GVizAttrs,
                            VertexData>>,

            // edge properties
            boost::property<
                    boost::edge_index_t, int, // edge index required for graphviz (?)
                    boost::property<
                            boost::edge_weight_t, int,
                            boost::property<
                                    boost::edge_attribute_t, GVizAttrs,
                                    EdgeData>>>,

            // graph properties
            boost::property<
                    boost::graph_name_t, std::string,
                    boost::property<
                            boost::graph_graph_attribute_t, GVizAttrs,
                            boost::property<
                                    boost::graph_vertex_attribute_t, GVizAttrs,
                                    boost::property<
                                            boost::graph_edge_attribute_t, GVizAttrs>>>>
    > FlowGraph;


    template<typename TGraph>
    void print_graph(const TGraph &g) {
        auto vert_attr_map = boost::get(boost::vertex_attribute, g);
        auto edge_attr_map = boost::get(boost::edge_attribute, g);

        auto vIterators = boost::vertices(g);
        for (auto it = vIterators.first; it != vIterators.second; it++) {

            auto vert = *it;

            std::cout << "Vertex: " << vert << ": ";

            for (auto &kv: vert_attr_map(vert)) {
                std::cout << "(" << kv.first << ": " << kv.second << ") ";
            }

            std::cout << std::endl;
        }

        auto eIterators = boost::edges(g);
        for (auto it = eIterators.first; it != eIterators.second; it++) {
            auto edge = *it;

            std::cout << "Edge: " << edge;

            for (auto &kv: edge_attr_map(edge)) {
                std::cout << "(" << kv.first << ": " << kv.second << ") ";
            }

            std::cout << std::endl;
        }
    }
}

#endif