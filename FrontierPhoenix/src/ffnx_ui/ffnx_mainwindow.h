#ifndef FFNX_UI_FFNX_MAINWINDOW_H
#define FFNX_UI_FFNX_MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QApplication>
#include "graph/graph_widget.h"

namespace ffnx::ui {
    class FFNXMainWindow : public QMainWindow {

        using TGraph = ffnx::graph::DirectedGraph<int, int>;

    public:
        explicit FFNXMainWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
            setWindowTitle("FFNX UI");

            auto graph = std::make_shared<TGraph>();

            auto v0 = graph->add_vertex();
            auto v1 = graph->add_vertex();
            auto v2 = graph->add_vertex();

            auto v3 = graph->add_vertex();
            auto v4 = graph->add_vertex();
            auto v5 = graph->add_vertex();

            graph->add_edge(v0, v1);
            graph->add_edge(v1, v2);
            graph->add_edge(v2, v0);

            graph->add_edge(v3, v4);
            graph->add_edge(v4, v5);
            graph->add_edge(v5, v3);

            graph->add_edge(v0, v3);
            graph->add_edge(v1, v4);
            graph->add_edge(v2, v5);

            auto engine = std::make_shared<graph::DefaultVertexPositioningEngine<TGraph>>(graph);

            auto *widget = new graph::GraphWidget<TGraph>(graph, engine, this);
            setCentralWidget(widget);
        }

    };
}

#endif