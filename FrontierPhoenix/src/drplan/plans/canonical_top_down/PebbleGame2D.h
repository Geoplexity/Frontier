#ifndef FRONTIER_PHOENIX_PEBBLEGAME_PEBBLEGAME2D
#define FRONTIER_PHOENIX_PEBBLEGAME_PEBBLEGAME2D

#include "graph/Graph.h"
#include "cluster/Cluster.h"
#include "PebbleGameGraph.h"
#include <boost/bimap.hpp>
#include <memory>
#include <stdexcept>
#include <stack>

/**
 * This is an attempt to understand the pebble game algorithm used by the DR planner here:
 *  FrontierPhoenix/canonical-top-down-dr-planner/src/Graph/Pebbled_Graph.cpp
 *
 *  Game appears to be played on a subset (cluster) of the graph.
 */
namespace ffnx::pebblegame {

    template<typename TGraph>
    class PebbleGame2D {
    private:

        static constexpr int PEBBLES_PER_VERTEX = 2;
        static constexpr int PEBBLES_PER_EDGE = 3;

        PebbleGameGraph<TGraph> pebble_game_graph;

        std::set<int> free_pebbles;

        using Cluster = cluster::Cluster<TGraph>;

        /**
         * Cluster that the game is played in.
         */
        std::weak_ptr<const Cluster> cluster;

    public:

        explicit PebbleGame2D(std::weak_ptr<const Cluster> cluster) :
            cluster(cluster),
            pebble_game_graph(PEBBLES_PER_VERTEX, PEBBLES_PER_EDGE, cluster.lock()->graph()) {
        }

        const PebbleGameGraph<TGraph>& get_game_graph() {
            return pebble_game_graph;
        }

        void run(const std::function<bool()>& callback) {
            // initializes game state
            free_pebbles.clear();
            pebble_game_graph = PebbleGameGraph<TGraph>(PEBBLES_PER_VERTEX, PEBBLES_PER_EDGE, cluster.lock()->graph());
            int pebble_id = 0;
            for (const auto& v : cluster.lock()->graph().lock()->vertices()) {
                pebble_game_graph.associate_vert(v);

                for (int i = 0; i < PEBBLES_PER_VERTEX; i++) {
                    free_pebbles.insert(pebble_id);
                    pebble_id++;
                    pebble_game_graph.place_pebble(free_pebbles, v);
                }
            }

            if (!callback()) {
                return;
            }

            /**
             * Keep executing until no more moves can be made.
             */
            while (step_game() && callback());
        }
    private:


        /**
         * @return true if cover of the specified edge could be enlarged.
         */
        bool step_game() {
            auto cluster_ptr = cluster.lock();
            auto graph = cluster_ptr->graph().lock();

            // add edge move
            // for i -> j where i has 2 and j has 2, create an edge ij and remove a pebble from i
            // look for an edge where both vertices have 2 pebbles
            for (const auto& e : cluster_ptr->edges()) {

                auto edge_verts = graph->vertices_for_edge(e);

                if (!cluster_ptr->includes_vertex(edge_verts.first) ||
                    !cluster_ptr->includes_vertex(edge_verts.second)) {
                    continue;
                }

                auto int_v0 = pebble_game_graph.internal_vert(edge_verts.first);
                auto int_v1 = pebble_game_graph.internal_vert(edge_verts.second);

                if (pebble_game_graph.get_vert_pebbles(int_v0).pebbles().size() == 2 &&
                    pebble_game_graph.get_vert_pebbles(int_v1).pebbles().size() == 2) {

                    pebble_game_graph.unplace_pebble(free_pebbles, int_v0);
                    pebble_game_graph.associate_edge(edge_verts.first, edge_verts.second);
                    return true;
                }
            }

            // pebble slide move
            // if ij is an edge and j has a pebble, reverse edge and move pebble from j to i
            for (const auto &int_edge : pebble_game_graph.graph().edges()) {
                auto int_verts = pebble_game_graph.graph().vertices_for_edge(int_edge);
                auto int_v0 = int_verts.first;
                auto int_v1 = int_verts.second;

                if (!pebble_game_graph.get_vert_pebbles(int_v1).pebbles().empty()) {
                    pebble_game_graph.remove_edge_association(int_v0, int_v1);
                    pebble_game_graph.associate_edge(int_v1, int_v0);

                    pebble_game_graph.unplace_pebble(free_pebbles, int_v1);
                    pebble_game_graph.place_pebble(free_pebbles, int_v0);
                    return true;
                }
            }

            return false;
        }

    };
}

#endif