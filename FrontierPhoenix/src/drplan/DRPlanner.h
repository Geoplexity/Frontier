#ifndef FRONTIER_PHOENIX_DRPLAN_DRPLANNER_H
#define FRONTIER_PHOENIX_DRPLAN_DRPLANNER_H

#include "flowgraph/Interface.h"
#include "drplan/DRPlan.h"

namespace ffnx::drplan {

    /**
     * Constructs a DRPlan based off the specified input flowgraph
     */
    template <typename TVert, typename TEdge>
    class DRPlanner {
    public:
        virtual DRPlan<TVert, TEdge> generate_plan(std::shared_ptr<const ffnx::flowgraph::FlowGraph<TVert, TEdge>> graph) = 0;
    };
}

#endif