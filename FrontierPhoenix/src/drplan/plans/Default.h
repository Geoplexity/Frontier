#ifndef FRONTIER_PHOENIX_DRPLAN_PLANS_DEFAULT_H
#define FRONTIER_PHOENIX_DRPLAN_PLANS_DEFAULT_H

#include "drplan/DRPlan.h"

namespace ffnx::drplan {

    template <typename TVert, typename TEdge>
    class DefaultDRPlan : public DRPlan<TVert, TEdge> {
    public:
        void solve(ffnx::flowgraph::FlowGraphInterface<TVert, TEdge> graph) override {
            // todo: start with copy of
            // FrontierPhoenix/canonical-top-down-dr-planner/src/Graph/DR_Plan.cpp

            // graph -> edges

            // original dr plan starts with separate cluster collections:
            // cluster_e, cluster_v

            // generate map edge : set(cluster_e)
            // map edge : (edge : cluster_e)

            // todo: incorporate pebblegame 2D


        }

    };
}

#endif