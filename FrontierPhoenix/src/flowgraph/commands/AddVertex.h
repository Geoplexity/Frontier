#ifndef FRONTIER_PHOENIX_FLOWGRAPH_COMMANDS_ADDVERTEX_H
#define FRONTIER_PHOENIX_FLOWGRAPH_COMMANDS_ADDVERTEX_H

#include "flowgraph/Command.h"

namespace ffnx::flowgraph::commands {

    template <typename TVD, typename TED>
    class AddVertexCommand : public FlowGraphCommand<TVD, TED> {
    private:
        typename FlowGraph<TVD, TED>::vertex_descriptor vert_desc;

        std::function<void(TVD&)> propertyInitializer;

    public:

        AddVertexCommand() : propertyInitializer([](TVD&){}) {

        }

        explicit AddVertexCommand(std::function<void(TVD&)> propertyInitializer) :
            propertyInitializer(propertyInitializer) {

        }

        const typename FlowGraph<TVD, TED>::vertex_descriptor& getVertex() {
            return vert_desc;
        }

    protected:

        void applyImpl(FlowGraph<TVD, TED> &flowGraph) {
            vert_desc = boost::add_vertex(flowGraph);
            propertyInitializer(flowGraph[vert_desc]);
        }

        void undoImpl(FlowGraph<TVD, TED> &flowGraph) {
            boost::remove_vertex(vert_desc, flowGraph);
        }
    };

}

#endif