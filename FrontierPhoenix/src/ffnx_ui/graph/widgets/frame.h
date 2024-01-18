#ifndef FFNX_UI_GRAPH_GRAPHWIDGET_FRAME
#define FFNX_UI_GRAPH_GRAPHWIDGET_FRAME

#include <map>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMouseEvent>
#include "../vertex.h"
#include "ffnx/graph/Interface.h"
#include "ffnx/graph/commands/AddVertex.h"
#include "ffnx/graph/commands/AddEdge.h"
#include "graphics_view.h"

// see example:
// https://doc.qt.io/qt-6/qtwidgets-graphicsview-elasticnodes-example.html

namespace ffnx::ui::graph {

    template <typename TGraph>
    class GraphFrame : public QFrame {
    private:

    public:
        GraphFrame(std::unique_ptr<ffnx::graph::GraphInterface<TGraph>> interface,
                   QWidget *parent = nullptr) : QFrame(parent) {

            auto layout = new QVBoxLayout(this);

            auto* graphics_view = new GraphGraphicsView<TGraph>(std::move(interface));

            layout->addWidget(graphics_view);

            layout->addWidget(new QLabel("test"));

            this->setLayout(layout);
            this->show();
        }
    };
}

#endif