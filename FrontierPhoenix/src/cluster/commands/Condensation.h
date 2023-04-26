#ifndef FRONTIER_PHOENIX_CLUSTER_CLUSTER_H
#define FRONTIER_PHOENIX_CLUSTER_CLUSTER_H

#include "cluster/Cluster.h"
#include "flowgraph/Command.h"

namespace ffnx::cluster::commands {

    template <typename TV, typename TE>
    class Condensation : public ffnx::flowgraph::FlowGraphCommand<TV, TE> {
    private:
        ffnx::cluster::Cluster<TV, TE> cluster;

    };
}

#endif