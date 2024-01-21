#ifndef FRONTIER_PHOENIX_FLOWGRAPH_COMMANDS_REMOVEVERTEX_H
#define FRONTIER_PHOENIX_FLOWGRAPH_COMMANDS_REMOVEVERTEX_H

#include <functional>
#include "ffnx/graph/Command.h"

namespace ffnx::graph::commands {

    template <typename TGraph>
    class RemoveVertexCommand : public GraphCommand<TGraph> {
    private:
        using vert_desc = typename TGraph::vertex_descriptor;

        vert_desc vertex;

    public:

        RemoveVertexCommand(vert_desc vertex) : vertex(vertex) {

        }

        const typename TGraph::vertex_descriptor& getVertex() const {
            return vertex;
        }

    protected:

        void applyImpl(TGraph &graph) {
            graph.remove_vertex(vertex);
        }

        void undoImpl(TGraph &graph) {
            vertex = graph.add_vertex();
        }
    };

}

#endif