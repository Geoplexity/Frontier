#ifndef FRONTIER_PHOENIX_PEBBLEGAME_COMPONENT_DETECTOR
#define FRONTIER_PHOENIX_PEBBLEGAME_COMPONENT_DETECTOR

#include "ffnx/drplan/plans/canonical_top_down/PebbleGameMove.h"
#include "ffnx/drplan/plans/canonical_top_down/PebbleGame2D.h"
#include "ffnx/cluster/Cluster.h"

#include <functional>
#include <set>
#include <queue>

namespace ffnx::pebblegame {


    template <typename TGraph>
    class ComponentQueue {
    private:
        using Vert = typename TGraph::vertex_descriptor;

        std::queue<Vert> queue;
        std::set<Vert> all_enqueued;

    public:
        bool is_empty() const {
            return queue.empty();
        }

        void enqueue_if_unvisited(const Vert& v) {
            if (!all_enqueued.contains(v)) {
                queue.push(v);
            }
        }

        template <typename ItBegin, typename ItEnd>
        void enqueue_if_unvisited(ItBegin begin, ItEnd end) {
            while (begin != end) {
                enqueue_if_unvisited(*begin);
                begin++;
            }
        }

        Vert dequeue() {
            auto result = queue.front();
            queue.pop();
            return result;
        }

    };

    /**
     *  from paper:
     *  Pebble game algorithms and sparse graphs
     *  Audrey Lee, Ileana Streinu, 2007
     */
    template <typename TGraph>
    class ComponentDetectorI : public PebbleGame2D<TGraph>::Callback {
    private:

        /**
         * Detected components.
         */
        std::vector<ffnx::cluster::Cluster<TGraph>> _components;

        std::weak_ptr<PebbleGame2D<TGraph>> _game;

    public:
        using Move = typename PebbleGame2D<TGraph>::Move;

        ComponentDetectorI(std::weak_ptr<PebbleGame2D<TGraph>> game) : _game(game), _components() {

        }

        std::vector<ffnx::cluster::Cluster<TGraph>>& components() {
            return _components;
        }

        /**
         * Process game events and identify components.
         */
        void process_move(const std::unique_ptr<Move>& move) {
            using Vert = typename PebbleGameGraph<TGraph>::InternalGraph::vertex_descriptor;

            // only process edge add moves
            if (move->type != Move::Type::EDGE_ADDED) {
                return;
            }

            std::cout << "Processing edge add" << std::endl;

            auto game_ptr = _game.lock();

            auto u = move->as_edge_added_move().v0;
            auto v = move->as_edge_added_move().v1;

            // more than L pebbles present on u and v, new edge is free
            int peb_count_u = game_ptr->get_game_graph().get_vert_pebble_count(u);
            int peb_count_v = game_ptr->get_game_graph().get_vert_pebble_count(v);

            if (peb_count_u + peb_count_v > game_ptr->l()) {
                // no component modifications
                std::cout << "peb count u (" << peb_count_u << ") + peb count v (" << peb_count_v << ") > l (" << game_ptr->l() << ")" << std::endl;
                return;
            }

            // compute the reach of the game vertices
            auto reach_u = game_ptr->get_game_graph().graph().compute_reach(u);
            auto reach_v = game_ptr->get_game_graph().graph().compute_reach(v);

            std::set<Vert> reach_uv;
            reach_uv.insert(reach_u->begin(), reach_u->end());
            reach_uv.insert(reach_v->begin(), reach_v->end());

            if (std::any_of(reach_uv.begin(), reach_uv.end(), [this](const auto &vertex){
                return has_free_pebble(vertex);
            })) {
                // no component modifications
                return;
            }

            // initialize V' = reach_uv
            std::set<Vert> v_prime = reach_uv;
            // initialize a queue and enqueue all vertices not in V' with an edge into V'
            ComponentQueue<typename PebbleGameGraph<TGraph>::InternalGraph> queue;
            enqueue_v_with_edge_into(queue, v_prime);

            while (!queue.is_empty()) {
                // dequeue vertex w from queue
                // compute reach_w
                // if all vertices in reach_w (other than u, v) have no free pebbles
                //      set v_prime = v_prime union reach_w
                //      enqueue all non-previously enqueued verts with edge into reach_w

                auto w = queue.dequeue();
                auto reach_w = game_ptr->get_game_graph().graph().compute_reach(w);
                if (std::all_of(reach_w->begin(), reach_w->end(), [this, u, v](auto &vertex){
                    return vertex == u || vertex == v || !this->has_free_pebble(vertex);
                })) {
                    v_prime.insert(reach_w->begin(), reach_w->end());
                    enqueue_v_with_edge_into(queue, *reach_w);
                }
            }

            // if l == 0, merge into V' the verts of existing component of G, if exists
            if (game_ptr->l() == 0) {
                throw std::runtime_error("l == 0 currently not supported");
            }

            // return V'
            std::set<typename TGraph::edge_descriptor> edges;
            ffnx::cluster::Cluster<TGraph> cluster(
                    game_ptr->cluster().lock()->graph(),
                    v_prime,
                    edges);

            _components.push_back(cluster);
        }

    private:
        bool has_free_pebble(const auto &v) const {
            return _game.lock()->get_game_graph().get_vert_pebble_count(v) > 0;
        }

        void enqueue_v_with_edge_into(ComponentQueue<typename PebbleGameGraph<TGraph>::InternalGraph> &queue,
                                      const std::set<typename TGraph::vertex_descriptor> &verts) {

            auto in_verts = v_with_edge_into(verts);

            for (const auto &v : *in_verts) {
                queue.enqueue_if_unvisited(v);
            }
        }

        auto v_with_edge_into(const std::set<typename TGraph::vertex_descriptor>& verts) {
            auto result = std::make_unique<std::set<typename PebbleGameGraph<TGraph>::InternalGraph::vertex_descriptor>>();

            auto game_ptr = _game.lock();

            for (const auto &e : game_ptr->get_game_graph().graph().edges()) {
                auto v0_v1 = game_ptr->get_game_graph().graph().vertices_for_edge(e);

                if (!verts.contains(v0_v1.first) && verts.contains(v0_v1.second)) {
                    result->insert(v0_v1.first);
                }
            }

            return std::move(result);
        }
    };
}

#endif