#ifndef FRONTIER_PHOENIX_CLUSTER_COMMANDS_CONDENSATION_H
#define FRONTIER_PHOENIX_CLUSTER_COMMANDS_CONDENSATION_H

#include "cluster/Cluster.h"
#include "flowgraph/Command.h"
#include "flowgraph/FlowGraph.h"

namespace ffnx::cluster::commands {

    template <typename TV, typename TE>
    using FlowGraph = ffnx::flowgraph::FlowGraph<TV, TE>;

    template <typename TV, typename TE>
    using VertDesc = boost::graph_traits<FlowGraph<TV, TE>>::vertex_descriptor;

    template <typename TV, typename TE>
    using EdgeDesc = boost::graph_traits<FlowGraph<TV, TE>>::edge_descriptor;

    template <typename TV, typename TE>
    class Condensation : public ffnx::flowgraph::FlowGraphCommand<TV, TE> {
    private:
        ffnx::cluster::Cluster<TV, TE> cluster;

    public:
        Condensation(const ffnx::cluster::Cluster<TV, TE>& cluster) : cluster(cluster) {

        }

    protected:

        /**
         * Removes all TV, TE from the graph, replacing them with a single condensed TV. Edges incident to vertices
         * outside the cluster are merged.
         */
        void applyImpl(FlowGraph<TV, TE> &graph) {
            // edges must be added to represent the connections to the cluster that were eliminated
            std::set<VertDesc<TV, TE>> externalSources;
            std::set<VertDesc<TV, TE>> externalTargets;

            for (const EdgeDesc<TV, TE>& e: cluster.getEdges()) {
                VertDesc<TV, TE> source = boost::source(e, graph);
                VertDesc<TV, TE> target = boost::target(e, graph);

                // todo:: copy edge properties!
                if (cluster.getVertices().find(source) != decltype(cluster.getVertices())::end) {
                    externalSources.insert(source);
                }

                if (cluster.getVertices().find(target) != decltype(cluster.getVertices())::end) {
                    externalTargets.insert(target);
                }
            }

            boost::graph_traits<FlowGraph<TV, TE>>::vertex_iterator vi, vi_end, next;
            boost::tie(vi, vi_end, next) = boost::vertices(graph);
            for (next = vi; vi != vi_end; vi = next) {
                next++;
                boost::remove_vertex(*vi, graph);
            }
        }

        void undoImpl(FlowGraph<TV, TE> &flowGraph) {

        }
    };
}

#endif