#ifndef FRONTIER_PHOENIX_PEBBLEGAME_PEBBLEGAME2D
#define FRONTIER_PHOENIX_PEBBLEGAME_PEBBLEGAME2D

#include "flowgraph/FlowGraph.h"
#include "cluster/Cluster.h"
#include <boost/bimap.hpp>
#include <memory>
#include <stdexcept>

/**
 * This is an attempt to understand the pebble game algorithm used by the DR planner here:
 *  FrontierPhoenix/canonical-top-down-dr-planner/src/Graph/Pebbled_Graph.cpp
 *
 *  Game appears to be played on a subset (cluster) of the graph.
 */
namespace ffnx::pebblegame {

    /**
     * Pebbles can be placed either on a vertex or an edge.
     */
    template <typename TVert, typename TEdge>
    class PebbleTracker {
    private:
        unsigned int num_pebbles;

        std::map<TVert, std::set<int>> vert_to_pebbles;
        std::map<int, TVert> pebble_to_vert;

        std::map<TEdge, std::set<int>> edge_to_pebbles;
        std::map<int, TEdge> pebble_to_edge;

    public:
        PebbleTracker(std::set<TVert>& verts) {

        }

        void place_pebble(const int& pebble, const TVert& vert) {
            if (pebble < 0 || pebble >= num_pebbles) {
                throw std::runtime_error("Invalid pebble index");
            }

            if (pebble_to_vert.contains(pebble)) {
                throw std::runtime_error("Pebble already associated with vertex");
            }

            pebble_to_vert[pebble] = vert;
            if (!vert_to_pebbles.contains(vert)) {
                vert_to_pebbles[vert] = {};
            }

            vert_to_pebbles[vert].insert(pebble);
        }
    };

    template<typename TV, typename TE>
    class PebbleGame2D {
    private:
        static constexpr int PEBBLES_PER_NODE = 2;

        using Pebble = int;

        using VertDesc = typename flowgraph::FlowGraph<TV, TE>::vertex_descriptor;
        using EdgeDesc = typename flowgraph::FlowGraph<TV, TE>::edge_descriptor;

        /**
         * Cluster that the game is played in.
         */
        std::weak_ptr<const cluster::Cluster<TV, TE>> cluster;

    public:

        PebbleGame2D(std::weak_ptr<const cluster::Cluster<TV, TE>> cluster) :
            cluster(cluster) {
            
        }

        void run_component() {
            // resetting the pebbles, to k pebbles per vertex
            // for each vertex:
            //      for i = 0 .. k - 1
            //      pebbles_at_vertex[vert][pebble] = [vert]
            // total pebbles = k * vertex count for subgraph

            // so each vertex gets k pebbles
            PebbleTracker<VertDesc, EdgeDesc> pebble_tracker(PEBBLES_PER_NODE *
                cluster.lock()->getVertices().size());

            int pebble_index = 0;
            for (const auto& v : cluster.lock()->getVertices()) {
                for (int i = 0; i < PEBBLES_PER_NODE; i++) {
                    pebble_tracker.place_pebble(pebble_index, v);
                    pebble_index++;
                }
            }

            // attempt to enlarge cover for each edge
            for (const auto& e : cluster.lock()->getEdges()) {
                enlarge_cover(e);
            }

            // determine the number of free pebbles in the end. if the
            // result is == 2, the graph is rigid
            pebble_tracker.available_pebble_count()
        }
    private:

        bool enlarge_cover(const EdgeDesc& edge) {
            // get incident vertices
            // if the vertex should be excluded, return false

            // let verts be v0, v1

            // attempt to identify free pebble for v0
            // will be an index from 0 .. k-1 (vertex capacity?)
        }

    };
}

#endif