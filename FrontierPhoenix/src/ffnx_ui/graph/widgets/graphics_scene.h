#ifndef FFNX_UI_GRAPH_GRAPHWIDGET_SCENE
#define FFNX_UI_GRAPH_GRAPHWIDGET_SCENE

#include <map>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMouseEvent>
#include "ffnx/graph/Interface.h"
#include "ffnx/graph/commands/AddVertex.h"
#include "ffnx/graph/commands/AddEdge.h"

// see example:
// https://doc.qt.io/qt-6/qtwidgets-graphicsview-elasticnodes-example.html

namespace ffnx::ui::graph {

    template <typename TGraph>
    class GraphGraphicsScene : public QGraphicsScene {
    private:
        std::shared_ptr<Controller<TGraph>> controller;

        using vert_desc = TGraph::vertex_descriptor;
        using edge_desc = TGraph::edge_descriptor;

        std::map<vert_desc, Vertex<TGraph>*> vert_scene_items;
        std::map<edge_desc, Edge<TGraph>*> edge_scene_items;

        std::map<vert_desc, std::unique_ptr<std::set<edge_desc>>> in_edges;
        std::map<vert_desc, std::unique_ptr<std::set<edge_desc>>> out_edges;

    public:

        GraphGraphicsScene(
                std::unique_ptr<ffnx::graph::GraphInterface<TGraph>> interface,
                QWidget* parent = nullptr) :
                    QGraphicsScene(parent) {

            this->controller = std::make_shared<Controller<TGraph>>(std::move(interface), this);
            this->controller->event_loop();
        }

        Vertex<TGraph>& get_vertex(const vert_desc& v) {
            if (!vert_scene_items.contains(v)) {
                throw std::runtime_error("Scene does not contain the specified vertex");
            }

            return *(vert_scene_items[v]);
        }

        void vertex_coordinate_changed(const vert_desc& v) {
            vert_scene_items[v]->recompute_position();

            for (const auto& e : *in_edges[v]) {
                edge_scene_items[e]->recompute_endpoints();
            }

            for (const auto& e : *out_edges[v]) {
                edge_scene_items[e]->recompute_endpoints();
            }

        }

        void add_vertex(Vertex<TGraph>* vertex) {
            if (vert_scene_items.contains(vertex->vertex_descriptor())) {
                throw std::runtime_error("Scene already contains a representation for this vertex");
            }

            vertex->set_controller(controller);
            vert_scene_items[vertex->vertex_descriptor()] = vertex;
            addItem(vertex);

            in_edges[vertex->vertex_descriptor()] = std::make_unique<std::set<edge_desc>>();
            out_edges[vertex->vertex_descriptor()] = std::make_unique<std::set<edge_desc>>();
        }

        void add_edge(Edge<TGraph>* edge) {
            if (edge_scene_items.contains(edge->edge_descriptor())) {
                throw std::runtime_error("Scene already contains a representation for this edge");
            }

            auto edge_verts = controller->interface().graph().vertices_for_edge(edge->edge_descriptor());
            if (!vert_scene_items.contains(edge_verts.first) || !vert_scene_items.contains(edge_verts.second)) {
                throw std::runtime_error("Attempted to add edge to scene without first adding vertices");
            }

            in_edges[edge_verts.second]->insert(edge->edge_descriptor());
            out_edges[edge_verts.first]->insert(edge->edge_descriptor());

            edge->set_controller(controller);
            edge->recompute_endpoints();
            edge_scene_items[edge->edge_descriptor()] = edge;
            addItem(edge);
        }

    protected:
        bool event(QEvent* event) {
            auto result = QGraphicsScene::event(event);

            if (result) {
                // trigger the controller to execute any pending commands
                controller->event_loop();
            }

            return result;
        }
    };

}

#endif