#ifndef FFNX_UI_GRAPH_GRAPHWIDGET
#define FFNX_UI_GRAPH_GRAPHWIDGET

#include <map>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMouseEvent>
#include "vertex.h"
#include "edge.h"
#include "vertex_positioning_engine.h"
#include "graph_command_factory.h"
#include "ffnx/graph/Interface.h"
#include "ffnx/graph/commands/AddVertex.h"
#include "ffnx/graph/commands/AddEdge.h"

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

        std::weak_ptr<GraphSelectionModel<TGraph>> selection_model;

        std::weak_ptr<GraphCommandFactory<TGraph>> command_factory;

        std::map<vert_desc, Vertex<TGraph>*> verts;
        std::map<edge_desc, Edge<TGraph>*> edges;

        std::optional<QPointF> user_position_input;

    public:
        GraphGraphicsView(std::weak_ptr<ffnx::graph::GraphInterface<TGraph>> interface,
                                   std::weak_ptr<VertexPositioningEngine<TGraph>> positioning_engine,
                                   std::weak_ptr<GraphSelectionModel<TGraph>> selection_model,
                                   std::weak_ptr<GraphCommandFactory<TGraph>> command_factory,
                                   QWidget* parent = nullptr) :
                                       interface(interface),
                                       positioning_engine(positioning_engine),
                                       selection_model(selection_model),
                                       command_factory(command_factory),
                                       QGraphicsView(parent) {

            auto* scene = new QGraphicsScene(this);
            scene->setItemIndexMethod(QGraphicsScene::NoIndex);
            scene->setSceneRect(0, 0, 400, 400);

            this->setScene(scene);
            setDragMode(RubberBandDrag);
            setCacheMode(CacheBackground);
            setViewportUpdateMode(BoundingRectViewportUpdate);
            setRenderHint(QPainter::Antialiasing);
            setTransformationAnchor(AnchorUnderMouse);

            // seems there is a buffer glitch when scale == 1 exactly...
            // Full viewport update mode also fixes this, but could be slow
            scale(qreal(0.9999999999), qreal(0.9999999999));
            setMinimumSize(400, 400);

            for (const auto &v : interface.lock()->graph().vertices()) {
                add_vertex_to_scene(v);
            }

            for (const auto& e : interface.lock()->graph().edges()) {
                add_edge_to_scene(e);
            }

            for (const auto& kv : verts) {
                kv.second->reposition();
            }

            interface.lock()->commandAppliedSubject().attachObserver([this](auto evt){
                auto add_vert = std::dynamic_pointer_cast<const typename ffnx::graph::commands::AddVertexCommand<TGraph>>(evt);

                if (add_vert != nullptr) {
                    this->add_vertex_to_scene(add_vert->getVertex());

                    for (const auto& kv : this->verts) {
                        kv.second->reposition();
                    }

                    if (user_position_input.has_value()) {

                        // vert is "tricked" into thinking it is selected and dragged
                        this->verts[add_vert->getVertex()]->setSelected(true);
                        this->verts[add_vert->getVertex()]->setPos(user_position_input.value().x(), user_position_input.value().y());
                        this->verts[add_vert->getVertex()]->setSelected(false);
                    }

                    return;
                }

                auto add_edge = std::dynamic_pointer_cast<const typename ffnx::graph::commands::AddEdgeCommand<TGraph>>(evt);
                if (add_edge != nullptr) {
                    add_edge_to_scene(add_edge->getEdge());

                    for (const auto& kv : verts) {
                        kv.second->reposition();
                    }
                    return;
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

        void mousePressEvent(QMouseEvent* evt) {
            //evt->position();
            if (evt->button() == Qt::RightButton) {
                user_position_input = evt->position();

                std::vector<std::shared_ptr<ffnx::graph::GraphCommand<TGraph>>> commands;
                command_factory.lock()->get_commands(commands);

                for (auto& c : commands) {
                    interface.lock()->applyCommand(c);
                }
            } else {
                QGraphicsView::mousePressEvent(evt);
            }
        }

    private:
        void add_vertex_to_scene(const TGraph::vertex_descriptor& v) {
            auto *vert = new Vertex<TGraph>(positioning_engine, selection_model, v);
            scene()->addItem(vert);
            verts[v] = vert;
        }

        void add_edge_to_scene(const TGraph::edge_descriptor& e) {
            auto *edge = new Edge<TGraph>(positioning_engine, selection_model, e);
            scene()->addItem(edge);
            edges[e] = edge;
        }
    };

    template <typename TGraph>
    class CommandHintLabel : public QLabel {
    private:
        std::weak_ptr<GraphSelectionModel<TGraph>> selection_model;

        std::weak_ptr<GraphCommandFactory<TGraph>> command_factory;

        ffnx::event::ObserverToken token;

    public:
        CommandHintLabel(std::weak_ptr<GraphSelectionModel<TGraph>> selection_model,
                         std::weak_ptr<GraphCommandFactory<TGraph>> command_factory,
                         QWidget* parent = nullptr) :
                             selection_model(selection_model),
                             command_factory(command_factory),
                             QLabel(parent) {

            token = selection_model.lock()->selection_changed_subject().attachObserver([&](){
                update_label();
            });

            update_label();
        }

        void update_label() {
            setText(QString::fromStdString(command_factory.lock()->get_command_suggestion()));
        }

    };

    template <typename TGraph>
    class GraphWidget : public QFrame {
    private:
        using interface_ptr = std::shared_ptr<ffnx::graph::GraphInterface<TGraph>>;

        interface_ptr graph_interface;

        std::shared_ptr<VertexPositioningEngine<TGraph>> positioning_engine;

        std::shared_ptr<GraphSelectionModel<TGraph>> selection_model = std::make_shared<GraphSelectionModel<TGraph>>();

        std::shared_ptr<GraphCommandFactory<TGraph>> command_factory = std::make_shared<GraphCommandFactory<TGraph>>(selection_model);

    public:
        GraphWidget(interface_ptr graph_interface,
                    std::shared_ptr<VertexPositioningEngine<TGraph>> positioning_engine,
                    QWidget *parent = nullptr) :
                        graph_interface(graph_interface),
                        positioning_engine(positioning_engine),
                        QFrame(parent) {

            auto layout = new QVBoxLayout(this);

            auto* graphics_view = new GraphGraphicsView<TGraph>(graph_interface,
                                                                positioning_engine,
                                                                selection_model,
                                                                command_factory);

            layout->addWidget(graphics_view);

            auto* label = new CommandHintLabel<TGraph>(selection_model, command_factory);

            layout->addWidget(label);

            this->setLayout(layout);
            this->show();
        }
    };
}

#endif