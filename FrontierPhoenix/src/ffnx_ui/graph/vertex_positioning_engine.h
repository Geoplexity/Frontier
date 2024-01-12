#ifndef FFNX_UI_GRAPH_VERTEX_POSITIONING_ENGINE
#define FFNX_UI_GRAPH_VERTEX_POSITIONING_ENGINE

#include <ffnx/graph/Interface.h>
#include <ogdf/basic/graph_generators.h>
#include <ogdf/layered/DfsAcyclicSubgraph.h>
#include <ogdf/planarity/SubgraphPlanarizer.h>
#include <ogdf/planarity/PlanarSubgraphFast.h>
#include <ogdf/planarity/VariableEmbeddingInserter.h>
#include <ogdf/layered/MedianHeuristic.h>
#include <ogdf/layered/OptimalHierarchyLayout.h>
#include <ogdf/layered/OptimalRanking.h>
#include <ogdf/layered/SugiyamaLayout.h>

namespace ffnx::ui::graph {

    template <typename TGraph>
    class VertexPositioningEngine {
    public:
        virtual std::pair<double, double> get_vertex_coordinate(const TGraph::vertex_descriptor &vdesc) const = 0;

        virtual std::pair<double, double> set_vertex_coordinate(const TGraph::vertex_descriptor &vdesc,
                                                                 const double &x,
                                                                 const double &y) = 0;

        virtual std::pair<double, double> get_edge_begin(const TGraph::edge_descriptor &edesc) const = 0;

        virtual std::pair<double, double> get_edge_end(const TGraph::edge_descriptor &edesc) const = 0;

        virtual void add_vert_listener(const TGraph::vertex_descriptor &vdesc, const std::function<void()> &callback) = 0;
        virtual void add_edge_listener(const TGraph::edge_descriptor &edesc, const std::function<void()> &callback) = 0;
    };

    template <typename TGraph>
    class DefaultVertexPositioningEngine : public VertexPositioningEngine<TGraph> {
    private:
        using graph_ptr = std::weak_ptr<TGraph>;
        graph_ptr graph;

        ogdf::Graph ogdf_graph;
        ogdf::GraphAttributes graph_attributes;

        std::map<typename TGraph::vertex_descriptor, ogdf::node> vert_map;
        std::map<typename TGraph::edge_descriptor, ogdf::edge> edge_map;

        std::map<typename TGraph::vertex_descriptor, std::unique_ptr<std::set<typename TGraph::edge_descriptor>>> edge_verts;

        std::map<typename TGraph::vertex_descriptor, std::function<void()>> vert_listeners;
        std::map<typename TGraph::edge_descriptor, std::function<void()>> edge_listeners;


    public:
        DefaultVertexPositioningEngine(graph_ptr graph) :
            graph(graph),
            ogdf_graph(),
            graph_attributes(ogdf_graph, ogdf::GraphAttributes::nodeGraphics | ogdf::GraphAttributes::edgeGraphics) {

            for (const auto &v : graph.lock()->vertices()) {
                auto node = ogdf_graph.newNode();
                vert_map[v] = node;
            }

            for (const auto &e : graph.lock()->edges()) {
                auto verts_for_edge = graph.lock()->vertices_for_edge(e);

                auto v0 = vert_map[verts_for_edge.first];
                auto v1 = vert_map[verts_for_edge.second];

                if (!edge_verts.contains(verts_for_edge.first)) {
                    edge_verts[verts_for_edge.first] = std::make_unique<std::set<typename TGraph::edge_descriptor>>();
                }

                if (!edge_verts.contains(verts_for_edge.second)) {
                    edge_verts[verts_for_edge.second] = std::make_unique<std::set<typename TGraph::edge_descriptor>>();
                }

                edge_verts[verts_for_edge.first]->insert(e);
                edge_verts[verts_for_edge.second]->insert(e);

                auto edge = ogdf_graph.newEdge(v0, v1);

                edge_map[e] = edge;
            }

            apply_layout();

            for (const auto& kv: vert_listeners) {
                kv.second();
            }

            for (const auto& kv: edge_listeners) {
                kv.second();
            }
        }

        void apply_layout() {
            ogdf::SugiyamaLayout SL;
            SL.setRanking(new ogdf::OptimalRanking);
            SL.setCrossMin(new ogdf::MedianHeuristic);

            ogdf::OptimalHierarchyLayout *ohl = new ogdf::OptimalHierarchyLayout;
            ohl->layerDistance(30.0);
            ohl->nodeDistance(25.0);
            ohl->weightBalancing(0.8);
            SL.setLayout(ohl);

            SL.call(graph_attributes);
        }

        std::pair<double, double> get_vertex_coordinate(const TGraph::vertex_descriptor &vdesc) const override {
            auto node = vert_map.at(vdesc);

            return {graph_attributes.x(node), graph_attributes.y(node)};
        }

        std::pair<double, double> set_vertex_coordinate(const TGraph::vertex_descriptor &vdesc, const double &x, const double &y) override {
            auto node = vert_map.at(vdesc);

            graph_attributes.x(node) = x;
            graph_attributes.y(node) = y;

            auto graph_locked = graph.lock();

            for (const auto& e : *(edge_verts[vdesc])) {
                if (edge_listeners.contains(e)) {
                    edge_listeners[e]();
                }
            }

            return {x, y};
        }

        std::pair<double, double> get_edge_begin(const TGraph::edge_descriptor &edesc) const override {
            auto verts_for_edge = graph.lock()->vertices_for_edge(edesc);
            return get_vertex_coordinate(verts_for_edge.first);
        }

        std::pair<double, double> get_edge_end(const TGraph::edge_descriptor &edesc) const override {
            auto verts_for_edge = graph.lock()->vertices_for_edge(edesc);
            return get_vertex_coordinate(verts_for_edge.second);
        }

        virtual void add_vert_listener(const TGraph::vertex_descriptor &vdesc, const std::function<void()> &callback) {
            vert_listeners[vdesc] = callback;
        };

        virtual void add_edge_listener(const TGraph::edge_descriptor &edesc, const std::function<void()> &callback) {
            edge_listeners[edesc] = callback;
        };

    };
}

#endif