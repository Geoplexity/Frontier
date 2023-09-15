#ifndef FRONTIER_PHOENIX_PEBBLEGAME_PEBBLEGAME2D
#define FRONTIER_PHOENIX_PEBBLEGAME_PEBBLEGAME2D

#include "flowgraph/FlowGraph.h"
#include "cluster/Cluster.h"
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
    public:

        class PebbleMovement {

            /**
             * If present, the vertex that the event applies to.
             */
            std::optional<TVert> _vertex;

            /**
             * If present, the edge that the event applies to.
             */
            std::optional<TEdge> _edge;

            /**
             * Indicates that the pebble was set-down ("placed") if true, otherwise removed if false.
             */
            bool _is_placed;

            /**
             * The pebble that was moved.
             */
            int _pebble_id;

        public:

            static PebbleMovement for_vertex(const TVert& vert, const bool& is_placed, const int& pebble_id) {
                PebbleMovement result;

                result._vertex = vert;
                result._pebble_id = pebble_id;
                result._is_placed = is_placed;

                return result;
            }

            static PebbleMovement for_edge(const TEdge& edge, const bool& is_placed, const int& pebble_id) {
                PebbleMovement result;

                result._edge = edge;
                result._pebble_id = pebble_id;
                result._is_placed = is_placed;

                return result;
            }

            int pebble_id() {
                return _pebble_id;
            }

            bool is_pebble_placed() {
                return _is_placed;
            }

            bool is_pebble_removed() {
                return !_is_placed;
            }

            bool applies_to_vertex() {
                return _vertex.has_value();
            }

            bool applies_to_edge() {
                return _edge.has_value();
            }

            TVert get_vertex() {
                if (!_vertex.has_value()) {
                    throw std::runtime_error("This event is edge-based, use get_edge to get the applicable edge.");
                }

                return _vertex.value();
            }

            TEdge get_edge() {
                if (!_edge.has_value()) {
                    throw std::runtime_error("This event is vertex-based, use get_vertex to get the applicable vertex.");
                }

                return _edge.value();
            }

        };

        /**
         * Multiple pebble micro-movements may be encapsulated in a single game move. For example unplace + place to
         * signify a movement from a vertex to an edge. Events are grouped in a vector to represent this.
         */
        using PebbleGameListener = std::function<void(const std::vector<typename PebbleTracker<TVert, TEdge>::PebbleMovement>&,
                                                      const PebbleTracker<TVert, TEdge>&)>;

    private:

        /**
         * Upper bound on number of pebbles.
         */
        unsigned int num_pebbles;

        /**
         * Max number of pebbles that can be placed on any vertex.
         */
        int _vertex_capacity;

        /**
         * Max number of pebbles that can be placed on an edge.
         */
        int _edge_capacity;

        std::map<TVert, std::set<int>> vert_to_pebbles;
        std::map<int, TVert> pebble_to_vert;

        std::map<TEdge, std::set<int>> edge_to_pebbles;
        std::map<int, TEdge> pebble_to_edge;

        PebbleGameListener listener;

        /**
         * Storage for micro-moves of the game. Once the listener is notified the queue is cleared.
         */
        std::vector<PebbleMovement> event_queue;

    public:
        PebbleTracker(const int& num_pebbles,
                      const int& vertex_capacity,
                      const int& edge_capacity,
                      const PebbleGameListener& listener = [](const auto &e, const auto &t){}) :
                        num_pebbles(num_pebbles),
                        _vertex_capacity(vertex_capacity),
                        _edge_capacity(edge_capacity),
                        listener(listener) {

        }

        [[nodiscard]] bool has_pending_events() const {
            return !event_queue.empty();
        }

        void assert_no_pending_events() const {
            if (has_pending_events()) {
                throw std::runtime_error("There are pending events in the queue. Remember to flush the queue before "
                                         "starting a new set of micro-moves.");
            }
        }

        /**
         * Notify listener of all pending events in the event queue. Each macro-movement may flush the event stack.
         * once all micro-moves are complete.
         */
        void flush_events() {
            if (!has_pending_events()) {
                throw std::runtime_error("No events to notify.");
            }

            listener(event_queue, *this);

            event_queue.clear();
        }

        [[nodiscard]] int vertex_capacity() const {
            return _vertex_capacity;
        }

        [[nodiscard]] int edge_capacity() const {
            return _edge_capacity;
        }

        [[nodiscard]] int free_pebble_count() const {
            return pebble_to_vert.size();
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

        bool is_vertex_saturated(const TVert& vert) const {
            return pebbles_on_vertex(vert) == _vertex_capacity;
        }

        bool is_edge_saturated(const TEdge& edge) const {
            return pebbles_on_edge(edge) == _edge_capacity;
        }

        /**
         * Transfers a single pebble, if possible, from vert to edge.
         * Exceptions are thrown when:
         * - the specified vertex does not have any pebbles to transfer
         * - the specified edge has already reached a capacity quantity of pebbles
         *
         * Callers should check that such a transfer is possible before calling
         * this method.
         */
        void transfer_vert_pebble_to_edge(const TVert& vert, const TEdge& edge) {
            assert_no_pending_events();

            int vert_pebble_count = pebbles_on_vertex(vert);
            if (vert_pebble_count == 0) {
                throw std::runtime_error("Specified vertex does not have any pebbles placed.");
            }

            int edge_pebble_count = pebbles_on_edge(edge);
            if (edge_pebble_count > this->_edge_capacity) {
                throw std::runtime_error("Internal Error: edge has exceeded capacity.");
            } else if (edge_pebble_count == this->_edge_capacity) {
                throw std::runtime_error("Destination edge has reached capacity.");
            }

            // it does not matter which element of the set is removed, so just use the first
            // that is found.
            int pebble_to_transfer = *(vert_to_pebbles[vert].begin());

            unplace_pebble(pebble_to_transfer);
            place_edge_pebble(pebble_to_transfer, edge);

            flush_events();
        }

        /**
         * Place the pebble on the specified vertex
         */
        void place_vert_pebble(const int& pebble, const TVert& vert) {
            assert_pebble_unplaced(pebble);
            if (pebbles_on_vertex(vert) == _vertex_capacity) {
                throw std::runtime_error("Vert already has maximum capacity");
            }

            pebble_to_vert[pebble] = vert;
            if (!vert_to_pebbles.contains(vert)) {
                vert_to_pebbles[vert] = {};
            }

            vert_to_pebbles[vert].insert(pebble);
            event_queue.push_back(PebbleMovement::for_vertex(vert, true, pebble));
        }

        /**
         * Place the pebble on the specified edge
         */
        void place_edge_pebble(const int& pebble, const TEdge& edge) {
            assert_pebble_unplaced(pebble);
            if (pebbles_on_edge(edge) == _edge_capacity) {
                throw std::runtime_error("Edge already has maximum capacity");
            }

            pebble_to_edge[pebble] = edge;
            if (!edge_to_pebbles.contains(edge)) {
                edge_to_pebbles[edge] = {};
            }

            edge_to_pebbles[edge].insert(pebble);
            event_queue.push_back(PebbleMovement::for_edge(edge, true, pebble));
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

                event_queue.push_back(PebbleMovement::for_vertex(vert, false, pebble));
            } else {
                auto edge = pebble_to_edge[pebble];

                pebble_to_edge.erase(pebble);

                edge_to_pebbles[edge].erase(pebble);

                event_queue.push_back(PebbleMovement::for_edge(edge, false, pebble));
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

    template <typename TVert, typename TEdge>
    class PebbleGameResult {
        int free_pebble_count;

    public:
        explicit PebbleGameResult(const int& free_pebble_count) : free_pebble_count(free_pebble_count) {

        }

        [[nodiscard]] bool is_rigid() const {
            return free_pebble_count == 2;
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

        explicit PebbleGame2D(std::weak_ptr<const cluster::Cluster<TV, TE>> cluster) :
            cluster(cluster) {
            
        }

        /**
         * @param result output variable.
         */
        std::unique_ptr<PebbleGameResult<VertDesc, EdgeDesc>> run(
                const typename PebbleTracker<VertDesc, EdgeDesc>::PebbleGameListener& listener) {
            // resetting the pebbles, to k pebbles per vertex
            // for each vertex:
            //      for i = 0 .. k - 1
            //      pebbles_at_vertex[vert][pebble] = [vert]
            // total pebbles = k * vertex count for subgraph

            // so each vertex gets k pebbles
            // todo: setting upper bound on num pebbles to int max for now, upper bound should either be removed
            // entirely or restricted to a lower value
            PebbleTracker<VertDesc, EdgeDesc> pebble_tracker(cluster.lock()->vertices().size() * PEBBLES_PER_VERTEX,
                                                             PEBBLES_PER_VERTEX,
                                                             PEBBLES_PER_EDGE,
                                                             listener);

            initialize(pebble_tracker);

            // attempt to enlarge cover for each edge
            for (const auto& e : cluster.lock()->edges()) {
                enlarge_cover(pebble_tracker, e);
            }

            // determine the number of free pebbles in the end. if the
            // result is == 2, the graph is rigid
            return std::move(std::make_unique<PebbleGameResult<VertDesc, EdgeDesc>>(
                    pebble_tracker.free_pebble_count()));
        }
    private:

        void initialize(PebbleTracker<VertDesc, EdgeDesc>& pebble_tracker) {
            int pebble_index = 0;

            // game should start with no pending events.
            pebble_tracker.assert_no_pending_events();

            for (const auto& v : cluster.lock()->vertices()) {
                for (int i = 0; i < PEBBLES_PER_VERTEX; i++) {
                    pebble_tracker.place_vert_pebble(pebble_index, v);
                    pebble_index++;
                }
            }

            pebble_tracker.flush_events();
        }

        /**
         * @return true if cover of the specified edge could be enlarged.
         */
        bool enlarge_cover(PebbleTracker<VertDesc, EdgeDesc>& pebble_tracker,
                           const EdgeDesc& edge) {

            if (pebble_tracker.is_edge_saturated(edge) || pebble_tracker.free_pebble_count() == 0) {
                // edge already saturated, coverage cannot be increased.
                // OR all pebbles are already on edges
                return false;
            }

            auto cluster_ptr = cluster.lock();
            auto graph_ptr = cluster_ptr->graph().lock();

            auto v0_v1 = graph_ptr->vertices_for_edge(edge);
            auto v0 = v0_v1.first;
            auto v1 = v0_v1.second;

            auto vertex_pebble_count = pebble_tracker.pebbles_on_vertex(v0);
            auto edge_pebble_count = pebble_tracker.pebbles_on_edge(edge);

            if (vertex_pebble_count > 0) {
                pebble_tracker.transfer_vert_pebble_to_edge(v0, edge);
                return true;
            } else {
                // make a pebble available for v0
                std::vector<VertDesc> path;
                bool path_found = reverse_edge_search(
                        path,
                        v0,
                        [&pebble_tracker](const auto& e){ return pebble_tracker.pebbles_on_edge(e) > 0; },
                        [&pebble_tracker](const auto& v){ return pebble_tracker.pebbles_on_vertex(v) > 0; });

                if (path_found) {
                    pebble_tracker.transfer_vert_pebble_to_edge(path[0], edge);
                    return true;
                }
            }

            return false;
        }

        /**
         * Performs a depth first edge search according to the specified predicate.
         */
        bool reverse_edge_search(std::vector<VertDesc> &path,
                                 const VertDesc& starting_vertex,
                                 std::function<bool(const EdgeDesc&)> edge_predicate,
                                 std::function<bool(const VertDesc&)> termination_predicate) {

            auto graph_ptr = cluster.lock()->graph().lock();

            std::stack<VertDesc> search_stack;
            search_stack.push(starting_vertex);
            std::set<VertDesc> visited_verts;
            visited_verts.insert(starting_vertex);

            while (!search_stack.empty()) {
                auto top_vert = search_stack.top();

                // search is terminated w. success.
                if (termination_predicate(top_vert)) {
                    path.push_back(top_vert);
                    while (!search_stack.empty()) {
                        path.push_back(search_stack.top());
                        search_stack.pop();
                    }
                    return true;
                }

                bool should_pop = true;
                for (const auto& v : graph_ptr->in_verts(top_vert)) {
                    if (!visited_verts.contains(v)) {
                        if (edge_predicate(graph_ptr->edge(v, top_vert))) {
                            search_stack.push(v);
                            should_pop = false;
                            break;
                        }
                    }
                }

                // no in_verts meeting required criteria, so pop the top off the stack and continue the search
                if (should_pop) {
                    search_stack.pop();
                }
            }

            return false;
        }
    };
}

#endif