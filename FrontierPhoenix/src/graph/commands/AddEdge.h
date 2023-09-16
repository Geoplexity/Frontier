#ifndef FRONTIER_PHOENIX_FLOWGRAPH_COMMANDS_ADDEDGE_H
#define FRONTIER_PHOENIX_FLOWGRAPH_COMMANDS_ADDEDGE_H

#include "graph/Command.h"

#include <exception>

namespace ffnx::graph::commands {

    template <typename TGraph>
    class AddEdgeCommand : public GraphCommand<TGraph> {
    private:

        typename TGraph::vertex_descriptor v0;
        typename TGraph::vertex_descriptor v1;

        typename TGraph::edge_descriptor edge_desc;

    public:

        AddEdgeCommand(
                typename TGraph::vertex_descriptor v0,
                typename TGraph::vertex_descriptor v1) :
            v0(v0), v1(v1) {
        }

        const typename TGraph::edge_descriptor& getEdge() {
            return edge_desc;
        }

    protected:

        void applyImpl(TGraph &graph) {
            auto add_edge = boost::add_edge(v0, v1, graph);
            if (!add_edge.second) {
                throw std::runtime_error("Could not create edge.");
            }

            edge_desc = add_edge.first;
        }

        void undoImpl(TGraph &graph) {
            boost::remove_edge(edge_desc, graph);
        }
    };

}

#endif