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
     * Responsible for recording the pebble-game state and checking
     * the rules are enforced.
     *
     * Pebbles can be placed either on a vertex or an edge.
     * The following states apply to a pebble:
     * Placed/Unplaced : is the pebble placed on an edge or vertex
     * Free/Not Free : free if on a vertex, not free if on an edge
     *
     * Free/Not Free state only defined for placed pebbles
     */
    template <typename TVert, typename TEdge>
    class PebbleTracker {
    private:

        /**
         * Upper bound on number of pebbles.
         */
        unsigned int num_pebbles;

        /**
         * Max number of pebbles that can be placed on any vertex.
         */
        int vertex_capacity;

        /**
         * Max number of pebbles that can be placed on an edge.
         */
        int edge_capacity;

        std::map<TVert, std::set<int>> vert_to_pebbles;
        std::map<int, TVert> pebble_to_vert;

        std::map<TEdge, std::set<int>> edge_to_pebbles;
        std::map<int, TEdge> pebble_to_edge;

    public:
        PebbleTracker(const int& num_pebbles,
                      const int& vertex_capacity,
                      const int& edge_capacity) : num_pebbles(num_pebbles),
                                                  vertex_capacity(vertex_capacity),
                                                  edge_capacity(edge_capacity) {

        }

        const std::map<int, TVert>& vertex_pebbles() const {
            return pebble_to_vert;
        }

        const std::map<int, TVert>& edge_pebbles() const {
            return pebble_to_edge;
        }

        int pebbles_on_vertex(const TVert& vert) const {
            if (!vert_to_pebbles.contains(vert)) {
                return 0;
            } else {
                return vert_to_pebbles.at(vert).size();
            }
        }

        int pebbles_on_edge(const TEdge& edge) const {
            if (!edge_to_pebbles.contains(edge)) {
                return 0;
            } else {
                return edge_to_pebbles.at(edge).size();
            }
        }

        /**
         * Place the pebble on the specified vertex
         */
        void place_vert_pebble(const int& pebble, const TVert& vert) {
            assert_pebble_unplaced(pebble);
            if (pebbles_on_vertex(vert) == vertex_capacity) {
                throw std::runtime_error("Vert already has maximum capacity");
            }

            pebble_to_vert[pebble] = vert;
            if (!vert_to_pebbles.contains(vert)) {
                vert_to_pebbles[vert] = {};
            }

            vert_to_pebbles[vert].insert(pebble);
        }

        /**
         * Place the pebble on the specified edge
         */
        void place_edge_pebble(const int& pebble, const TEdge& edge) {
            assert_pebble_unplaced(pebble);
            if (pebbles_on_edge(edge) == edge_capacity) {
                throw std::runtime_error("Edge already has maximum capacity");
            }

            pebble_to_edge[pebble] = edge;
            if (!edge_to_pebbles.contains(edge)) {
                vert_to_pebbles[edge] = {};
            }

            edge_to_pebbles[edge].insert(pebble);
        }

        /**
         * Removes the specified pebble from the vertex or edge that it is placed on
         */
        void unplace_pebble(const int& pebble) {
            assert_pebble_placed(pebble);

            if (pebble_to_vert.contains(pebble)) {
                auto vert = pebble_to_vert[pebble];

                pebble_to_vert.erase(pebble);

                vert_to_pebbles[vert].erase(vert);
            } else {
                auto edge = pebble_to_edge(pebble);

                pebble_to_edge.erase(edge);

                edge_to_pebbles[edge].erase(pebble);
            }
        }

    private:
        void assert_pebble_in_range(const int& pebble) const {
            if (pebble < 0 || pebble >= num_pebbles) {
                throw std::runtime_error("Invalid pebble index");
            }
        }

        void assert_pebble_free(const int& pebble) const {
            assert_pebble_in_range(pebble);
            assert_pebble_placed(pebble);

            if (!pebble_to_vert.contains(pebble)) {
                throw std::runtime_error("Pebble not free");
            }
        }

        void assert_pebble_not_free(const int& pebble) const {
            assert_pebble_in_range(pebble);
            assert_pebble_placed(pebble);

            if (!pebble_to_edge.contains(pebble)) {
                throw std::runtime_error("Pebble is free");
            }
        }

        void assert_pebble_placed(const int& pebble) const {
            assert_pebble_in_range(pebble);
        }

        void assert_pebble_unplaced(const int& pebble) const {
            assert_pebble_in_range(pebble);

            if (pebble_to_vert.contains(pebble)) {
                throw std::runtime_error("Pebble already associated with vertex");
            }

            if (pebble_to_edge.contains(pebble)) {
                throw std::runtime_error("Pebble already associated with edge");
            }
        }
    };

    template<typename TV, typename TE>
    class PebbleGame2D {
    private:

        static constexpr int PEBBLES_PER_VERTEX = 2;
        static constexpr int PEBBLES_PER_EDGE = 3;

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
            // todo: setting upper bound on num pebbles to int max for now, upper bound should either be removed
            // entirely or restricted to a lower value
            PebbleTracker<VertDesc, EdgeDesc> pebble_tracker(std::numeric_limits<int>::max(),
                                                             PEBBLES_PER_VERTEX,
                                                             PEBBLES_PER_EDGE);

            int pebble_index = 0;
            for (const auto& v : cluster.lock()->vertices()) {
                for (int i = 0; i < PEBBLES_PER_VERTEX; i++) {
                    pebble_tracker.place_vert_pebble(pebble_index, v);
                    pebble_index++;
                }
            }

            // attempt to enlarge cover for each edge
            for (const auto& e : cluster.lock()->edges()) {
                enlarge_cover(e);
            }

            // determine the number of free pebbles in the end. if the
            // result is == 2, the graph is rigid
            pebble_tracker.available_pebble_count();

            throw std::runtime_error("Not yet implemented");
        }
    private:

        bool enlarge_cover(const EdgeDesc& edge) {
            // get incident vertices
            // if the vertex should be excluded, return false

            // let verts be v0, v1

            // attempt to identify free pebble for v0
            // will be an index from 0 .. k-1 (vertex capacity?)

            throw std::runtime_error("Not implemented");
        }

    };
}

#endif