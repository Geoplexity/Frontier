#ifndef FRONTIER_PHOENIX_DRPLAN_DRPLAN_H
#define FRONTIER_PHOENIX_DRPLAN_DRPLAN_H

#include "flowgraph/Interface.h"

namespace ffnx::drplan {

    /**
     * Operates on a flowgraph. Output should be a list of
     * FlowGraphCommands, indicating the required mutations
     * to the graph.
     */
    template <typename TVert, typename TEdge>
    class DRPlan {
    public:
        virtual void solve(ffnx::flowgraph::FlowGraphInterface<TVert, TEdge>& graph) = 0;
    };
}

#endif