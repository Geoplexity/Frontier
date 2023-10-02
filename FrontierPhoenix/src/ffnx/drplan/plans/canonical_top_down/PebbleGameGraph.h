#ifndef FRONTIER_PHOENIX_PEBBLEGAME_PEBBLEGAMEGRAPH
#define FRONTIER_PHOENIX_PEBBLEGAME_PEBBLEGAMEGRAPH

#include "ffnx/graph/Graph.h"
#include <stdexcept>

namespace ffnx::pebblegame {
/**
 * Maintains an association between pebbles and an arbitrary data type.
 */

    using PebbleStack = std::vector<int>;

    class PebbleMapping {
    private:
        int capacity;

        PebbleStack _pebbles;

    public:
        explicit PebbleMapping(const int &capacity) : capacity(capacity) {

        }

        [[nodiscard]] const PebbleStack &pebbles() const {
            return _pebbles;
        }

        void take_pebble(PebbleStack &free_pebbles) {
            if (_pebbles.size() == capacity) {
                throw std::runtime_error("Already saturated with pebbles.");
            }

            if (free_pebbles.empty()) {
                throw std::runtime_error("No free pebbles to take.");
            }

            int pebble = free_pebbles.back();
            free_pebbles.pop_back();
            _pebbles.push_back(pebble);
        }

        void give_pebble(PebbleStack &free_pebbles) {
            if (_pebbles.empty()) {
                throw std::runtime_error("No pebbles to give.");
            }

            int pebble_to_give = _pebbles.back();
            _pebbles.pop_back();

            free_pebbles.push_back(pebble_to_give);
        }

        void transfer_pebble(PebbleMapping &other) {
            if (_pebbles.empty()) {
                throw std::runtime_error("No pebbles to transfer");
            }

            other.take_pebble(_pebbles);
        }
    };

/**
 * All mappings assume the input graph is not modified.
 * vertex mappings are 1:1
 * edge mappings are 1:1
 *
 * once associated, vertex mappings are permanent
 * edge mappings may be removed and re-applied with different directionality.
 */
    template<typename TGraph>
    class PebbleGameGraph {
    public:
        using InternalGraph = ffnx::graph::DirectedGraph<
                typename TGraph::vertex_descriptor,
                typename TGraph::edge_descriptor>;


        using ext_vert = typename TGraph::vertex_descriptor;
        using ext_edge = typename TGraph::edge_descriptor;

        using int_vert = typename InternalGraph::vertex_descriptor;
        using int_edge = typename InternalGraph::edge_descriptor;

    private:
        std::weak_ptr<const TGraph> input_graph;

        InternalGraph internal_graph;

        std::map<ext_vert, int_vert> vertex_mappings;
        std::map<ext_edge, int_edge> edge_mappings;

        std::map<int_vert, PebbleMapping> vertex_pebbles;
        std::map<int_edge, PebbleMapping> edge_pebbles;

        int vertex_capacity;
        int edge_capacity;

    public:

        PebbleGameGraph(const int &vertex_capacity, const int &edge_capacity, std::weak_ptr<const TGraph> input_graph) :
                vertex_capacity(vertex_capacity),
                edge_capacity(edge_capacity),
                input_graph(input_graph) {

        }

        const InternalGraph &graph() const {
            return internal_graph;
        }

        const PebbleMapping &get_vert_pebbles(const int_vert &v) const {
            assert_vertex_associated(v);
            return vertex_pebbles.at(v);
        }

        int get_vert_pebble_count(const ext_vert &v) const {
            return vertex_pebbles.at(internal_vert(v)).pebbles().size();
        }

        const PebbleMapping &get_edge_pebbles(const int_edge &e) const {
            if (!edge_pebbles.contains(e)) {
                throw std::runtime_error("Edge not associated");
            }

            return edge_pebbles.at(e);
        }

        int get_edge_pebble_count(const ext_edge &e) const {
            return edge_pebbles.at(internal_edge(e)).pebbles().size();
        }

        void place_pebble(PebbleStack &free_pebbles, const ext_vert &v) {
            assert_vertex_associated(v);
            vertex_pebbles.at(internal_vert(v)).take_pebble(free_pebbles);
        }

        void place_pebble(PebbleStack &free_pebbles, const ext_vert &v0, const ext_vert &v1) {
            assert_vertex_associated(v0);
            assert_vertex_associated(v1);
            edge_pebbles.at(internal_edge(v0, v1)).take_pebble(free_pebbles);
        }

        void unplace_pebble(PebbleStack &free_pebbles, const ext_vert &v) {
            assert_vertex_associated(v);
            vertex_pebbles.at(internal_vert(v)).give_pebble(free_pebbles);
        }

        void unplace_pebble(PebbleStack &free_pebbles, const ext_vert &v0, const ext_vert &v1) {
            assert_vertex_associated(v0);
            assert_vertex_associated(v1);
            edge_pebbles.at(internal_edge(v0, v1)).give_pebble(free_pebbles);
        }

        /**
         * Causes an association to be generated between the specified vertex and a vertex in the internal graph.
         */
        void associate_vert(const ext_vert &external_vert) {
            if (vertex_mappings.contains(external_vert)) {
                throw std::runtime_error("Vertex already associated");
            }

            auto internal_vert = internal_graph.add_vertex();
            internal_graph[internal_vert] = external_vert;
            vertex_mappings[external_vert] = internal_vert;
            vertex_pebbles.emplace(internal_vert, PebbleMapping(vertex_capacity));
        }

        ext_vert external_vert(const int_vert &internal_vert) const {
            return internal_graph[internal_vert];
        }

        int_vert internal_vert(const ext_vert &external_vert) const {
            return vertex_mappings.at(external_vert);
        }

        ext_edge external_edge(const int_edge &internal_edge) const {
            return internal_graph[internal_edge];
        }

        int_edge internal_edge(const ext_edge &external_edge) const {
            return edge_mappings.at(external_edge);
        }

        int_edge internal_edge(const ext_vert &v0, const ext_vert &v1) const {
            auto int_v0 = internal_vert(v0);
            auto int_v1 = internal_vert(v1);

            return internal_graph.edge(int_v0, int_v1);
        }

        void associate_edge(const ext_vert &from, const ext_vert &to) {
            assert_vertex_associated(from);
            assert_vertex_associated(to);

            auto internal_v0 = vertex_mappings[from];
            auto internal_v1 = vertex_mappings[to];

            if (internal_graph.has_edge(internal_v1, internal_v0)) {
                throw std::runtime_error("A reverse edge is already present.");
            }

            auto external_edge_desc = input_graph.lock()->edge(from, to);

            if (edge_mappings.contains(external_edge_desc)) {
                throw std::runtime_error("Edge already has an association.");
            }

            auto internal_edge_desc = internal_graph.add_edge(internal_v0, internal_v1);

            internal_graph[internal_edge_desc] = external_edge_desc;
            edge_mappings[external_edge_desc] = internal_edge_desc;

            edge_pebbles.emplace(internal_edge_desc, PebbleMapping(edge_capacity));
        }

        void remove_edge_association(const typename TGraph::vertex_descriptor &from,
                                     const typename TGraph::vertex_descriptor &to) {
            assert_vertex_associated(from);
            assert_vertex_associated(to);

            auto internal_v0 = vertex_mappings[from];
            auto internal_v1 = vertex_mappings[to];

            auto internal_edge_desc = internal_graph.edge(internal_v0, internal_v1);
            auto external_edge_desc = internal_graph[internal_edge_desc];

            if (edge_pebbles.at(internal_edge_desc).pebbles().size() != 0) {
                throw std::runtime_error("Edge cannot be disassociated because it still has pebbles.");
            }

            edge_mappings.erase(external_edge_desc);
            internal_graph.remove_edge(internal_edge_desc);
        }

    private:
        void assert_vertex_associated(const ext_vert &vert) const {
            if (!vertex_mappings.contains(vert)) {
                throw std::runtime_error("External vertex has not been associated.");
            }
        }
    };
}


#endif