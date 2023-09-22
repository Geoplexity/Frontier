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
    public:

        using VertDesc = typename TGraph::vertex_descriptor;
        using EdgeDesc = typename TGraph::edge_descriptor;

        struct Move {
            std::optional<std::pair<VertDesc, VertDesc>> edge_being_added = std::nullopt;

            // entities directly affected by the move
            std::set<VertDesc> verts;
            std::set<EdgeDesc> edges;

            // associated entities
            std::set<VertDesc> dfs_verts;
            std::set<EdgeDesc> dfs_edges;

            std::vector<std::string> messages;

            static std::unique_ptr<Move> dfs_search(const VertDesc &v0,
                                                    const VertDesc &v1,
                                                    const std::vector<VertDesc> &dfs_verts,
                                                    const std::vector<EdgeDesc> &dfs_edges) {
                auto result = std::make_unique<Move>();

                result->edge_being_added = {0, 0};
                result->edge_being_added.value().first = v0;
                result->edge_being_added.value().second = v1;

                for (const auto& e : dfs_edges) {
                    result->dfs_edges.insert(e);
                }

                for (const auto& v : dfs_verts) {
                    result->dfs_verts.insert(v);
                }

                result->messages.push_back("dfs");

                return result;
            }

            static std::unique_ptr<Move> indicate_target_edge(const VertDesc &v0, const VertDesc &v1) {
                auto result = std::make_unique<Move>();

                result->edge_being_added = {0, 0};
                result->edge_being_added.value().first = v0;
                result->edge_being_added.value().second = v1;

                result->messages.push_back("Selecting target edge");

                return result;
            }

            /**
             * @param target_v0 vertex of edge affected by move
             * @param target_v1 vertex of edge affected by move
             * @param v0 vertex of edge aiming to be added
             * @param v1 vertex of edge aiming to be added
             * @param e
             * @param dfs_verts
             * @param dfs_edges
             * @return
             */
            static std::unique_ptr<Move> pebble_slide_move(const VertDesc &target_v0,
                                                           const VertDesc &target_v1,
                                                           const VertDesc &v0,
                                                           const VertDesc &v1,
                                                           const EdgeDesc &e,
                                                           const std::set<VertDesc> &dfs_verts,
                                                           const std::vector<EdgeDesc> &dfs_edges) {
                auto result = std::make_unique<Move>();

                result->edge_being_added = {0, 0};
                result->edge_being_added.value().first = target_v0;
                result->edge_being_added.value().second = target_v1;

                for (const auto &v : dfs_verts) {
                    result->dfs_verts.insert(v);
                }

                for (const auto &edge : dfs_edges) {
                    result->dfs_edges.insert(edge);
                }

                result->messages.push_back("Pebble Slide Move");

                return result;
            }

            static std::unique_ptr<Move> edge_added(const VertDesc &v0, const VertDesc &v1, const EdgeDesc &e) {
                auto result = std::make_unique<Move>();
                result->edge_being_added = {0, 0};
                result->edge_being_added.value().first = v0;
                result->edge_being_added.value().second = v1;

                result->verts.insert(v0);
                result->verts.insert(v1);
                result->edges.insert(e);

                result->messages.push_back("Add Edge Move");

                return result;
            }
        };

        using Callback = std::function<void(std::unique_ptr<Move>)>;

    private:

        static constexpr int PEBBLES_PER_VERTEX = 2;
        static constexpr int PEBBLES_PER_EDGE = 3;

        static constexpr int TARGET_PEBBLE_COUNT = PEBBLES_PER_EDGE + 1;

        PebbleGameGraph<TGraph> pebble_game_graph;

        std::vector<int> free_pebbles;

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

        void run(const Callback& callback) {
            // initializes game state
            free_pebbles = std::vector<int>();
            pebble_game_graph = PebbleGameGraph<TGraph>(PEBBLES_PER_VERTEX, PEBBLES_PER_EDGE, cluster.lock()->graph());
            int pebble_id = 0;
            for (const auto& v : cluster.lock()->graph().lock()->vertices()) {
                pebble_game_graph.associate_vert(v);

                for (int i = 0; i < PEBBLES_PER_VERTEX; i++) {
                    free_pebbles.push_back(pebble_id);
                    pebble_id++;
                    pebble_game_graph.place_pebble(free_pebbles, v);
                }
            }

            auto move = std::make_unique<Move>();
            move->messages.push_back("Initial State");
            callback(std::move(move));

            auto cluster_ptr = cluster.lock();
            auto graph_ptr = cluster_ptr->graph().lock();

            for (const auto &input_edge : cluster_ptr->edges()) {
                auto v0_v1 = graph_ptr->vertices_for_edge(input_edge);
                auto v0 = std::get<0>(v0_v1);
                auto v1 = std::get<1>(v0_v1);
                if (!cluster_ptr->includes_vertex(v0) || !cluster_ptr->includes_vertex(v1)) {
                    throw std::runtime_error("Cluster contains a \"dangling\" edge. The edge belongs to the cluster "
                                             "but one of its vertices does not.");
                }

                step_game(input_edge, callback);
            }

        }
    private:


        /**
         * @return true if cover of the specified edge could be enlarged.
         */
        void step_game(const EdgeDesc& edge, const Callback& callback) {
            // game iteration
            // for each input edge:
            // search available pebbles
            //    if no >= l+1 edge add move
            //  else:
            //    dfs the edges we've added to see if
            //    there is an available pebble somewhere, if there is record this path
            //    and perform the edge slide move to move it all the way back to this edge
            //    keep doing this until we meet the edge critera or cannot find any pebbles
            //    to collect

            //    circuits?? = computeCorrespondingSet

            auto cluster_ptr = cluster.lock();
            auto graph = cluster_ptr->graph().lock();

            VertDesc v0;
            VertDesc v1;
            boost::tie(v0, v1) = graph->vertices_for_edge(edge);

            callback(Move::indicate_target_edge(v0, v1));

            // first establish the number of pebbles currently on each vertex
            int starting_pebble_count =
                    pebble_game_graph.get_vert_pebble_count(v0) + pebble_game_graph.get_vert_pebble_count(v1);
            if (starting_pebble_count >= TARGET_PEBBLE_COUNT) {
                perform_add_edge_move(v0, v1, callback);
                return;
            }

            // otherwise we need to find N pebbles
            if (collect_pebbles_depth_first_search(v0, v1, TARGET_PEBBLE_COUNT, callback)) {
                perform_add_edge_move(v0, v1, callback);
            }
        }

    private:
        void perform_add_edge_move(VertDesc v0, VertDesc v1, const Callback& callback) {
            pebble_game_graph.associate_edge(v0, v1);
            pebble_game_graph.unplace_pebble(free_pebbles, v0);

            callback(Move::edge_added(
                    v0,
                    v1,
                    pebble_game_graph.external_edge(pebble_game_graph.graph().edge(v0, v1))));
        }

        void perform_edge_slide_move(
                VertDesc target_v0,
                VertDesc target_v1,
                VertDesc v0,
                VertDesc v1,
                const std::set<VertDesc> &processed_verts,
                const std::vector<EdgeDesc> & processed_edges,
                const Callback& callback) {
            if (!pebble_game_graph.graph().has_edge(v0, v1)) {
                throw std::runtime_error("No edge exists for v0 -> v1");
            }

            pebble_game_graph.remove_edge_association(v0, v1);
            pebble_game_graph.associate_edge(v1, v0);

            pebble_game_graph.unplace_pebble(free_pebbles, v1);
            pebble_game_graph.place_pebble(free_pebbles, v0);

            auto edge = pebble_game_graph.external_edge(pebble_game_graph.graph().edge(v1, v0));

            callback(Move::pebble_slide_move(target_v0, target_v1, v0, v1, edge, processed_verts, processed_edges));
        }

        bool collect_pebbles_depth_first_search(
                const VertDesc& v0,
                const VertDesc& v1,
                const int& required_pebbles,
                const Callback& callback) {

            while (true) {

                int acquired_pebbles =
                        pebble_game_graph.get_vert_pebble_count(v0) + pebble_game_graph.get_vert_pebble_count(v1);

                if (acquired_pebbles >= required_pebbles) {
                    return true;
                }

                bool collected_on_v0 = collect_single_pebble_depth_first(v0, v1, callback);

                acquired_pebbles =
                        pebble_game_graph.get_vert_pebble_count(v0) + pebble_game_graph.get_vert_pebble_count(v1);

                if (acquired_pebbles >= required_pebbles) {
                    return true;
                }

                bool collected_on_v1 = collect_single_pebble_depth_first(v1, v0, callback);

                if (!collected_on_v0 && !collected_on_v1) {
                    // no longer able to increase the pebble coverage
                    auto move = std::make_unique<Move>();
                    move->verts.insert(pebble_game_graph.external_vert(v0));
                    move->verts.insert(pebble_game_graph.external_vert(v1));
                    move->messages.push_back("Coverage increase failed");
                    callback(std::move(move));
                    return false;
                }
            }

        }

        std::unique_ptr<std::vector<EdgeDesc>> int_stack_to_ext_edges(const std::vector<typename PebbleGameGraph<TGraph>::int_vert>& verts) {
            auto result = std::make_unique<std::vector<EdgeDesc>>();

            for (int i = 1; i < verts.size(); i++) {
                auto v0 = pebble_game_graph.external_vert(verts[i - 1]);
                auto v1 = pebble_game_graph.external_vert(verts[i]);

                auto edge_int = pebble_game_graph.internal_edge(v0, v1);
                result->push_back(pebble_game_graph.external_edge(edge_int));
            }

            return result;
        }

        std::unique_ptr<std::vector<VertDesc>> int_stack_to_ext_verts(const std::vector<typename PebbleGameGraph<TGraph>::int_vert>& verts) {
            auto result = std::make_unique<std::vector<VertDesc>>();

            for (const auto& v : verts) {
                result->push_back(v);
            }

            return result;
        }

        /**
         * @param target_vertex the vertex that a pebble should be moved towards.
         * @param ignored_vertex the pair-vertex (vertex that shares the same edge) to ignore
         */
        bool collect_single_pebble_depth_first(
                const VertDesc& target_vertex,
                const VertDesc& ignored_vertex,
                const Callback& callback) {

            std::vector<typename PebbleGameGraph<TGraph>::int_vert> exploration_stack;
            std::set<typename PebbleGameGraph<TGraph>::int_vert> visited_verts;

            exploration_stack.push_back(target_vertex);
            visited_verts.insert(target_vertex);
            visited_verts.insert(ignored_vertex);

            while (!exploration_stack.empty()) {
                auto current_vert = exploration_stack.back();

                auto edges = int_stack_to_ext_edges(exploration_stack);
                auto verts = int_stack_to_ext_verts(exploration_stack);
                callback(Move::dfs_search(
                        pebble_game_graph.external_vert(target_vertex),
                        pebble_game_graph.external_vert(ignored_vertex),
                        *verts,
                        *edges));

                for (const auto& v : pebble_game_graph.graph().out_verts(current_vert)) {
                    if (!visited_verts.contains(v)) {
                        exploration_stack.push_back(v);
                        visited_verts.insert(v);
                        break;
                    }
                }

                if (exploration_stack.back() != current_vert) {
                    // show that the stack was changed
                    edges = int_stack_to_ext_edges(exploration_stack);
                    verts = int_stack_to_ext_verts(exploration_stack);
                    callback(Move::dfs_search(
                            pebble_game_graph.external_vert(target_vertex),
                            pebble_game_graph.external_vert(ignored_vertex),
                            *verts,
                            *edges));
                }

                if (exploration_stack.back() == current_vert) {
                    // no next vertex was found, this is a dead end.
                    exploration_stack.pop_back();
                    continue;
                } else if (pebble_game_graph.get_vert_pebble_count(exploration_stack.back()) > 0) {
                    // a vertex with a pebble has been found. We can stop and unroll the stack.
                    break;
                }
            }

            if (exploration_stack.empty()) {
                // unable to find a pebble
                return false;
            }

            // the stack now contains the sequence of edges to be reversed in order to return a pebble to
            // the target vertex.
            std::set<VertDesc> processed_verts;
            for (const auto &v : visited_verts) {
                processed_verts.insert(pebble_game_graph.external_vert(v));
            }
            std::set<EdgeDesc> processed_edges;

            while (exploration_stack.size() > 1) {
                auto v1 = exploration_stack.back();
                exploration_stack.pop_back();

                auto v0 = exploration_stack.back();

                auto stack = int_stack_to_ext_edges(exploration_stack);

                perform_edge_slide_move(target_vertex, ignored_vertex, v0, v1, {}, *stack, callback);
            }

            return true;
        }
    };
}

#endif