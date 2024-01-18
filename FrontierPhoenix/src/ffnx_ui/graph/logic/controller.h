#ifndef FFNX_UI_GRAPH_CONTROLLER
#define FFNX_UI_GRAPH_CONTROLLER

#include "ui_command.h"
#include "vertex_positioning_engine.h"
#include "graph_selection_model.h"

namespace ffnx::ui::graph {

    template <typename TGraph>
    class GraphGraphicsScene;

    template <typename TGraph>
    class Controller : public std::enable_shared_from_this<Controller<TGraph>> {
    private:
        std::unique_ptr<VertexPositioningEngine<TGraph>> _positioning_engine;

        std::unique_ptr<GraphSelectionModel<TGraph>> _selection_model;

        std::shared_ptr<ffnx::graph::GraphInterface<TGraph>> graph_interface;

        std::vector<std::unique_ptr<UIGraphCommand<TGraph>>> command_stack;

        GraphGraphicsScene<TGraph>* _scene;

        // index of the next command to be executed
        int pending_command_index = 0;

        bool _is_event_loop_running = false;

    public:
        Controller(std::unique_ptr<ffnx::graph::GraphInterface<TGraph>> graph_interface,
                   GraphGraphicsScene<TGraph>* scene) :
            graph_interface(),
            _scene(scene) {

            this->graph_interface = std::move(graph_interface);

            _positioning_engine = std::make_unique<DefaultVertexPositioningEngine<TGraph>>(this->graph_interface);
            _selection_model = std::make_unique<GraphSelectionModel<TGraph>>();

            // queue commands for adding the required scene elements/positioning
            for (const auto& v : this->graph_interface->graph().vertices()) {
                push_command(std::make_unique<AddVertexToSceneUICommand<TGraph>>(v));
            }

            for (const auto& e : this->graph_interface->graph().edges()) {
                push_command(std::make_unique<AddEdgeToSceneUICommand<TGraph>>(e));
            }

            push_command(std::make_unique<ResetVertexPositionsCommand<TGraph>>());
        }

        void push_command(std::unique_ptr<UIGraphCommand<TGraph>> command) {
            // trigger the event loop based off this command
            command_stack.push_back(std::move(command));
        }

        void push_command(std::unique_ptr<UICommand<TGraph>> command) {
            push_command(std::make_unique<UIGraphCommand<TGraph>>(std::move(command)));
        }

        void push_command(std::unique_ptr<ffnx::graph::GraphCommand<TGraph>> command) {
            push_command(std::make_unique<UIGraphCommand<TGraph>>(std::move(command)));
        }

        /**
         * If true, listeners triggered by the controller or UI events may avoid infinite loops by checking if their
         * adjustment is taking place as part of the event loop.
         */
        bool is_event_loop_running() const {
            return _is_event_loop_running;
        }

        VertexPositioningEngine<TGraph>& positioning_engine() {
            return *_positioning_engine;
        }

        GraphSelectionModel<TGraph>& selection_model() {
            return *_selection_model;
        }

        GraphGraphicsScene<TGraph>& scene() {
            return *_scene;
        }

        const ffnx::graph::GraphInterface<TGraph>& interface() const {
            return *this->graph_interface;
        }

        void event_loop() {
            _is_event_loop_running = true;

            while (pending_command_index < command_stack.size()) {

                if (command_stack[pending_command_index]->is_ui_command()) {
                    ui_command_event_loop(command_stack[pending_command_index]->ui_command());
                } else {
                    graph_command_event_loop(command_stack[pending_command_index]->graph_command());
                }

                pending_command_index++;
            }

            _is_event_loop_running = false;
        }

    private:

        void ui_command_event_loop(UICommand<TGraph>& ui_command) {
            ui_command.apply(*this);
        }

        void graph_command_event_loop(std::shared_ptr<ffnx::graph::GraphCommand<TGraph>> command) {
            graph_interface->applyCommand(command);
        }
    };
}

#endif