#ifndef FFNX_UI_GRAPH_UI_UI_COMMAND_H
#define FFNX_UI_GRAPH_UI_UI_COMMAND_H

#include "ffnx/graph/Command.h"

namespace ffnx::ui::graph {

    template <typename TGraph>
    class Controller;

    template <typename TGraph>
    class Vertex;

    template <typename TGraph>
    class Edge;

    /**
     * Represents a UI change (e.g. vertex repositioning/selection change) that does not affect the underlying graph
     * structure.
     */
    template <typename TGraph>
    class UICommand {
    public:
        virtual void apply(Controller<TGraph>& controller) = 0;

        virtual void undo(Controller<TGraph>& controller) = 0;
    };

    /**
     * Represents either a UICommand or a GraphCommand which affects the underlying graph.
     */
    template <typename TGraph>
    class UIGraphCommand {
    private:
        std::unique_ptr<UICommand<TGraph>> _ui_command;
        std::shared_ptr<ffnx::graph::GraphCommand<TGraph>> _graph_command;


    public:
        UIGraphCommand(std::unique_ptr<UICommand<TGraph>> ui_command) :
                _ui_command(std::move(ui_command)),
                _graph_command(nullptr) {

        }

        UIGraphCommand(std::unique_ptr<ffnx::graph::GraphCommand<TGraph>> graph_command) :
            _ui_command(nullptr),
            _graph_command(graph_command) {

        }

        bool is_ui_command() const {
            return _ui_command != nullptr;
        }

        UICommand<TGraph>& ui_command() {
            if (!is_ui_command()) {
                throw std::runtime_error("Not a UI command");
            }

            return *_ui_command;
        }

        std::shared_ptr<ffnx::graph::GraphCommand<TGraph>> graph_command() {
            if (is_ui_command()) {
                throw std::runtime_error("Not a Graph command");
            }

            return _graph_command;
        }
    };

    template <typename TGraph>
    class VertexPositioningUICommand : public UICommand<TGraph> {
    private:
        typename TGraph::vertex_descriptor vertex;

        double x;

        double y;

    public:
        VertexPositioningUICommand(typename TGraph::vertex_descriptor vertex, double x, double y) : vertex(vertex), x(x), y(y) {

        }

        void apply(Controller<TGraph>& controller) {
            controller.positioning_engine().set_vertex_coordinate(vertex, x, y);
            controller.scene().vertex_coordinate_changed(vertex);
        }

        void undo(Controller<TGraph>& controller) {
            throw std::runtime_error("Not implemented");
        }
    };

    template <typename TGraph>
    class VertexSelectionUICommand : public UICommand<TGraph> {
    private:
        typename TGraph::vertex_descriptor vertex;

        bool state;

    public:

        VertexSelectionUICommand(typename TGraph::vertex_descriptor vertex, bool state) : vertex(vertex), state(state) {

        }

        void apply(Controller<TGraph>& controller) {
            if (state) {
                controller.selection_model().select_vertex(vertex);
            } else {
                controller.selection_model().deselect_vertex(vertex);
            }
        }

        void undo(Controller<TGraph>& controller) {
            throw std::runtime_error("Not implemented");
        }
    };

    template <typename TGraph>
    class AddVertexToSceneUICommand : public UICommand<TGraph> {
    private:
        typename TGraph::vertex_descriptor vertex;

    public:

        AddVertexToSceneUICommand(typename TGraph::vertex_descriptor vertex) : vertex(vertex) {

        }

        void apply(Controller<TGraph>& controller) {
            controller.scene().add_vertex(new Vertex<TGraph>(vertex));
        }

        void undo(Controller<TGraph>& controller) {
            throw std::runtime_error("Not implemented");
        }
    };


    template <typename TGraph>
    class AddEdgeToSceneUICommand : public UICommand<TGraph> {
    private:
        typename TGraph::edge_descriptor edge;

    public:

        AddEdgeToSceneUICommand(typename TGraph::edge_descriptor edge) : edge(edge) {

        }

        void apply(Controller<TGraph>& controller) {
            controller.scene().add_edge(new Edge<TGraph>(edge));
        }

        void undo(Controller<TGraph>& controller) {
            throw std::runtime_error("Not implemented");
        }
    };

    template <typename TGraph>
    class ResetVertexPositionsCommand : public UICommand<TGraph> {
    private:

    public:


        void apply(Controller<TGraph>& controller) {
            controller.positioning_engine().apply_layout();

            for (const auto& v : controller.interface().graph().vertices()) {
                auto coord = controller.positioning_engine().get_vertex_coordinate(v);
                controller.scene().get_vertex(v).setPos(qreal(coord.first), qreal(coord.second));
            }
        }

        void undo(Controller<TGraph>& controller) {
            throw std::runtime_error("Not implemented");
        }
    };
};

#endif