#ifndef FRONTIER_PHOENIX_FLOWGRAPH_COMMANDS_ADDVERTEX_H
#define FRONTIER_PHOENIX_FLOWGRAPH_COMMANDS_ADDVERTEX_H

#include <functional>
#include "ffnx/graph/Command.h"

namespace ffnx::graph::commands {

    template <typename TGraph>
    class AddVertexCommand : public GraphCommand<TGraph> {
    private:
        typename TGraph::vertex_descriptor vert_desc;

        using vval = TGraph::vertex_data_type;

        std::function<void(vval&)> propertyInitializer;

    public:

        AddVertexCommand() : propertyInitializer([](vval&){}) {

        }

        explicit AddVertexCommand(std::function<void(vval&)> propertyInitializer) :
            propertyInitializer(propertyInitializer) {

        }

        const typename TGraph::vertex_descriptor& getVertex() const {
            return vert_desc;
        }

    protected:

        void applyImpl(TGraph &graph) {
            vert_desc = boost::add_vertex(graph);
            propertyInitializer(graph[vert_desc]);
        }

        void undoImpl(TGraph &graph) {
            boost::remove_vertex(vert_desc, graph);
        }
    };

}

#endif