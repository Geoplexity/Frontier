#ifndef FRONTIER_PHOENIX_DRPLAN_DRPLANNER_H
#define FRONTIER_PHOENIX_DRPLAN_DRPLANNER_H

#include "flowgraph/Interface.h"
#include "drplan/DRPlan.h"

namespace ffnx::drplan {

    /**
     * Constructs a DRPlan based off the specified input graph
     */
    template <typename TVert, typename TEdge, typename TDir>
    class DRPlanner {
    public:
        virtual DRPlan<TVert, TEdge, TDir> generate_plan(std::shared_ptr<const ffnx::graph::Graph<TVert, TEdge, TDir>> graph) = 0;
    };
}

#endif