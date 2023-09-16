#ifndef FRONTIER_PHOENIX_FLOWGRAPH_COMMANDS_ADDEDGE_H
#define FRONTIER_PHOENIX_FLOWGRAPH_COMMANDS_ADDEDGE_H

#include "flowgraph/Command.h"

#include <exception>

namespace ffnx::graph::commands {

    template <typename TVD, typename TED>
    class AddEdgeCommand : public FlowGraphCommand<TVD, TED> {
    private:
        typename FlowGraph<TVD, TED>::vertex_descriptor v0;
        typename FlowGraph<TVD, TED>::vertex_descriptor v1;

        typename FlowGraph<TVD, TED>::edge_descriptor edge_desc;

    public:

        AddEdgeCommand(
                typename FlowGraph<TVD, TED>::vertex_descriptor v0,
                typename FlowGraph<TVD, TED>::vertex_descriptor v1) :
            v0(v0), v1(v1) {
        }

        const typename FlowGraph<TVD, TED>::edge_descriptor& getEdge() {
            return edge_desc;
        }

    protected:

        void applyImpl(FlowGraph<TVD, TED> &flowGraph) {
            auto add_edge = boost::add_edge(v0, v1, flowGraph);
            if (!add_edge.second) {
                throw std::runtime_error("Could not create edge.");
            }

            edge_desc = add_edge.first;
        }

        void undoImpl(FlowGraph<TVD, TED> &flowGraph) {
            boost::remove_edge(edge_desc, flowGraph);
        }
    };

}

#endif