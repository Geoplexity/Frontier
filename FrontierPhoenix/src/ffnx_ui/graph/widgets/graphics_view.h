#ifndef FFNX_UI_GRAPH_GRAPHWIDGET_VIEW
#define FFNX_UI_GRAPH_GRAPHWIDGET_VIEW

#include <map>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMouseEvent>
#include "../vertex.h"
#include "../edge.h"
#include "graphics_scene.h"
#include "ffnx/graph/Interface.h"
#include "ffnx/graph/commands/AddVertex.h"
#include "ffnx/graph/commands/AddEdge.h"

// see example:
// https://doc.qt.io/qt-6/qtwidgets-graphicsview-elasticnodes-example.html

namespace ffnx::ui::graph {

    template <typename TGraph>
    class GraphGraphicsView : public QGraphicsView {
    private:

    public:
        GraphGraphicsView(std::unique_ptr<ffnx::graph::GraphInterface<TGraph>> interface, QWidget* parent = nullptr) :
            QGraphicsView(parent) {

            auto* scene = new GraphGraphicsScene<TGraph>(std::move(interface), this);
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