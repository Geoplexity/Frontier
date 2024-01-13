#ifndef FFNX_UI_GRAPH_GRAPHWIDGET
#define FFNX_UI_GRAPH_GRAPHWIDGET

#include <map>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include "vertex.h"
#include "edge.h"
#include "vertex_positioning_engine.h"
#include "ffnx/graph/Interface.h"
#include "ffnx/graph/commands/AddVertex.h"

// see example:
// https://doc.qt.io/qt-6/qtwidgets-graphicsview-elasticnodes-example.html

namespace ffnx::ui::graph {

    template <typename TGraph>
    class GraphGraphicsView : public QGraphicsView {
    private:

        using vert_desc = typename TGraph::vertex_descriptor;
        using edge_desc = typename TGraph::edge_descriptor;

        std::weak_ptr<ffnx::graph::GraphInterface<TGraph>> interface;

        std::weak_ptr<VertexPositioningEngine<TGraph>> positioning_engine;

        std::map<vert_desc, Vertex<TGraph>*> verts;
        std::map<edge_desc, Edge<TGraph>*> edges;

    public:
        GraphGraphicsView(std::weak_ptr<ffnx::graph::GraphInterface<TGraph>> interface,
                                   std::weak_ptr<VertexPositioningEngine<TGraph>> positioning_engine,
                                   QWidget* parent = nullptr) :
                                       interface(interface),
                                       positioning_engine(positioning_engine),
                                       QGraphicsView(parent) {

            auto* scene = new QGraphicsScene(this);
            scene->setItemIndexMethod(QGraphicsScene::NoIndex);
            scene->setSceneRect(0, 0, 400, 400);

            this->setScene(scene);
            setCacheMode(CacheBackground);
            setViewportUpdateMode(BoundingRectViewportUpdate);
            setRenderHint(QPainter::Antialiasing);
            setTransformationAnchor(AnchorUnderMouse);

            // seems there is a buffer glitch when scale == 1 exactly...
            // Full viewport update mode also fixes this, but could be slow
            scale(qreal(0.9999999999), qreal(0.9999999999));
            setMinimumSize(400, 400);
            setWindowTitle(tr("Test"));

            for (const auto &v : interface.lock()->graph().vertices()) {
                add_vertex_to_scene(v);
            }

            for (const auto& e : interface.lock()->graph().edges()) {
                add_edge_to_scene(e);
            }

            for (const auto& kv : verts) {
                kv.second->reposition();
            }

            interface.lock()->commandAppliedSubject().attachObserver([&](auto evt){
                auto add_vert = std::dynamic_pointer_cast<const typename ffnx::graph::commands::AddVertexCommand<TGraph>>(evt);

                if (add_vert != nullptr) {
                    add_vertex_to_scene(add_vert->getVertex());

                    for (const auto& kv : verts) {
                        kv.second->reposition();
                    }
                }
            });
        }

        void drawBackground(QPainter *painter, const QRectF &rect) override {
            auto scene_rect = this->sceneRect();

            QLinearGradient gradient(rect.topLeft(), rect.bottomRight());
            gradient.setColorAt(0, Qt::white);
            gradient.setColorAt(1, Qt::lightGray);

            painter->fillRect(scene_rect, gradient);
        }

    private:
        void add_vertex_to_scene(const TGraph::vertex_descriptor& v) {
            auto *vert = new Vertex<TGraph>(positioning_engine, v);
            scene()->addItem(vert);
            verts[v] = vert;
        }

        void add_edge_to_scene(const TGraph::edge_descriptor& e) {
            auto *edge = new Edge<TGraph>(positioning_engine, e);
            scene()->addItem(edge);
            edges[e] = edge;
        }
    };

    template <typename TGraph>
    class GraphWidget : public QFrame {
    private:
        using interface_ptr = std::shared_ptr<ffnx::graph::GraphInterface<TGraph>>;

        interface_ptr graph_interface;

        std::shared_ptr<VertexPositioningEngine<TGraph>> positioning_engine;


    public:
        GraphWidget(interface_ptr graph_interface,
                    std::shared_ptr<VertexPositioningEngine<TGraph>> positioning_engine,
                    QWidget *parent = nullptr) :
                        graph_interface(graph_interface),
                        positioning_engine(positioning_engine),
                        QFrame(parent) {

            auto layout = new QVBoxLayout(this);

            auto* graphics_view = new GraphGraphicsView<TGraph>(graph_interface, positioning_engine);

            layout->addWidget(graphics_view);

            auto* label = new QLabel("instructions/state info here");
            auto* add_vertex = new QPushButton("Add vertex");

            connect(add_vertex, &QPushButton::released, this, &GraphWidget::add_vertex_button);

            layout->addWidget(add_vertex);

            this->setLayout(layout);
            this->show();
        }

        void add_vertex_button() {
            graph_interface->applyCommand(std::make_shared<ffnx::graph::commands::AddVertexCommand<TGraph>>());
        }
    };
}

#endif