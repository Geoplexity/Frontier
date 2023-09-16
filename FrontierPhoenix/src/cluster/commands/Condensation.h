#ifndef FRONTIER_PHOENIX_CLUSTER_COMMANDS_CONDENSATION_H
#define FRONTIER_PHOENIX_CLUSTER_COMMANDS_CONDENSATION_H

#include <boost/graph/adjacency_list.hpp>

#include "cluster/Cluster.h"
#include "graph/Command.h"
#include "graph/Graph.h"

namespace ffnx::cluster::commands {

    template <typename Graph>
    class Condensation : public ffnx::graph::GraphCommand<Graph> {
    private:

        using VertDesc = Graph::vertex_descriptor;

        using EdgeDesc = Graph::edge_descriptor;

        ffnx::cluster::Cluster<Graph> cluster;

    public:
        Condensation(const ffnx::cluster::Cluster<Graph>& cluster) : cluster(cluster) {

        }

    protected:

        /**
         * Removes all TV, TE from the graph, replacing them with a single condensed TV. Edges incident to vertices
         * outside the cluster are merged.
         */
        void applyImpl(Graph &graph) {
            // edges must be added to represent the connections to the cluster that were eliminated
            std::set<VertDesc> externalSources;
            std::set<VertDesc> externalTargets;

            for (const EdgeDesc& e: cluster.getEdges()) {
                VertDesc source = boost::source(e, graph);
                VertDesc target = boost::target(e, graph);

                // todo:: copy edge properties!
                if (cluster.getVertices().find(source) != decltype(cluster.getVertices())::end) {
                    externalSources.insert(source);
                }

                if (cluster.getVertices().find(target) != decltype(cluster.getVertices())::end) {
                    externalTargets.insert(target);
                }
            }

            typename boost::graph_traits<Graph>::vertex_iterator vi, vi_end, next;
            boost::tie(vi, vi_end, next) = boost::vertices(graph);
            for (next = vi; vi != vi_end; vi = next) {
                next++;
                boost::remove_vertex(*vi, graph);
            }
        }

        void undoImpl(Graph &flowGraph) {

        }
    };
}

#endif