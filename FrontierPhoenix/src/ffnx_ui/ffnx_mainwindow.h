#ifndef FFNX_UI_FFNX_MAINWINDOW_H
#define FFNX_UI_FFNX_MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QApplication>
#include "graph/widgets/frame.h"

#include <ffnx/graph/Graph.h>
#include <ffnx/graph/commands/AddVertex.h>
#include <ffnx/graph/commands/AddEdge.h>

namespace ffnx::ui {
    class FFNXMainWindow : public QMainWindow {

        using TGraph = ffnx::graph::DirectedGraph<int, int>;
        using AddVertex = ffnx::graph::commands::AddVertexCommand<TGraph>;
        using AddEdge = ffnx::graph::commands::AddEdgeCommand<TGraph>;

    public:
        explicit FFNXMainWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
            setWindowTitle("FFNX UI");

            auto interface = std::make_unique<ffnx::graph::GraphInterface<TGraph>>();

            auto v0 = interface->applyCommand(std::make_shared<AddVertex>())->getVertex();
            auto v1 = interface->applyCommand(std::make_shared<AddVertex>())->getVertex();
            auto v2 = interface->applyCommand(std::make_shared<AddVertex>())->getVertex();

            auto v3 = interface->applyCommand(std::make_shared<AddVertex>())->getVertex();
            auto v4 = interface->applyCommand(std::make_shared<AddVertex>())->getVertex();
            auto v5 = interface->applyCommand(std::make_shared<AddVertex>())->getVertex();

            interface->applyCommand(std::make_shared<AddEdge>(v0, v1));
            interface->applyCommand(std::make_shared<AddEdge>(v1, v2));
            interface->applyCommand(std::make_shared<AddEdge>(v2, v0));

            interface->applyCommand(std::make_shared<AddEdge>(v3, v4));
            interface->applyCommand(std::make_shared<AddEdge>(v4, v5));
            interface->applyCommand(std::make_shared<AddEdge>(v5, v3));

            interface->applyCommand(std::make_shared<AddEdge>(v0, v3));
            interface->applyCommand(std::make_shared<AddEdge>(v1, v4));
            interface->applyCommand(std::make_shared<AddEdge>(v2, v5));

            //auto engine = std::make_shared<graph::DefaultVertexPositioningEngine<TGraph>>(interface);

            auto *widget = new graph::GraphFrame<TGraph>(std::move(interface), this);
            setCentralWidget(widget);
        }

    };
}

#endif