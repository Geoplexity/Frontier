#ifndef FFNX_UI_GRAPH_GRAPHWIDGET
#define FFNX_UI_GRAPH_GRAPHWIDGET

#include <map>
#include <QGraphicsView>
#include "vertex.h"
#include "edge.h"
#include "vertex_positioning_engine.h"
#include "ffnx/graph/Interface.h"

// see example:
// https://doc.qt.io/qt-6/qtwidgets-graphicsview-elasticnodes-example.html

namespace ffnx::ui::graph {

    template <typename TGraph>
    class GraphWidget : public QGraphicsView {
    private:
        using graph_ptr = std::weak_ptr<TGraph>;

        using vert_desc = typename TGraph::vertex_descriptor;
        using edge_desc = typename TGraph::edge_descriptor;

        graph_ptr graph;

        std::shared_ptr<VertexPositioningEngine<TGraph>> positioning_engine;

        std::map<vert_desc, Vertex<TGraph>*> verts;
        std::map<edge_desc, Edge<TGraph>*> edges;

    public:
        GraphWidget(graph_ptr graph,
                    std::shared_ptr<VertexPositioningEngine<TGraph>> positioning_engine,
                    QWidget *parent = nullptr) :
                        graph(graph),
                        positioning_engine(positioning_engine),
                        QGraphicsView(parent) {

            QGraphicsScene *scene = new QGraphicsScene(this);
            scene->setItemIndexMethod(QGraphicsScene::NoIndex);
            scene->setSceneRect(0, 0, 400, 400);

            this->setScene(scene);
            setCacheMode(CacheBackground);
            setViewportUpdateMode(BoundingRectViewportUpdate);
            setRenderHint(QPainter::Antialiasing);
            setTransformationAnchor(AnchorUnderMouse);
            scale(qreal(0.8), qreal(0.8));
            setMinimumSize(400, 400);
            setWindowTitle(tr("Test"));

            for (const auto &v : graph.lock()->vertices()) {
                auto *vert = new Vertex<TGraph>(positioning_engine, v);
                scene->addItem(vert);
                verts[v] = vert;
            }

            for (const auto& e : graph.lock()->edges()) {
                auto *edge = new Edge<TGraph>(positioning_engine, e);
                scene->addItem(edge);
                edges[e] = edge;
            }

            for (const auto& kv : verts) {
                kv.second->reposition();
            }
        }

        void drawBackground(QPainter *painter, const QRectF &rect) override {
            auto scene_rect = this->sceneRect();

            QLinearGradient gradient(rect.topLeft(), rect.bottomRight());
            gradient.setColorAt(0, Qt::white);
            gradient.setColorAt(1, Qt::lightGray);

            painter->fillRect(scene_rect, gradient);
        }
    };
}

#endif