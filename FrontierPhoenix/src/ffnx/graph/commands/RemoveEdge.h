#ifndef FRONTIER_PHOENIX_FLOWGRAPH_COMMANDS_REMOVEEDGE_H
#define FRONTIER_PHOENIX_FLOWGRAPH_COMMANDS_REMOVEEDGE_H

#include <functional>
#include "ffnx/graph/Command.h"

namespace ffnx::graph::commands {

    template <typename TGraph>
    class RemoveEdgeCommand : public GraphCommand<TGraph> {
    private:
        using vert_desc = typename TGraph::vertex_descriptor;
        using edge_desc = typename TGraph::edge_descriptor;

        vert_desc v0;
        vert_desc v1;

        edge_desc e;

    public:

        // todo: a bit messy because vert/edge descriptors could potentially be set to conflicting values
        RemoveEdgeCommand(vert_desc v0, vert_desc v1, edge_desc e) : v0(v0), v1(v1), e(e) {

        }

        const typename TGraph::vertex_descriptor& getV0() const {
            return v0;
        }

        const typename TGraph::vertex_descriptor& getV1() const {
            return v1;
        }

        const typename TGraph::edge_descriptor& getEdge() const {
            return e;
        }

    protected:

        void applyImpl(TGraph &graph) {
            graph.remove_edge(v0, v1);
        }

        void undoImpl(TGraph &graph) {
            graph.add_edge(v0, v1);
        }
    };

}

#endif