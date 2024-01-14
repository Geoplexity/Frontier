#ifndef FFNX_UI_GRAPH_GRAPHCOMMANDFACTORY
#define FFNX_UI_GRAPH_GRAPHCOMMANDFACTORY

#include "ffnx/graph/Interface.h"
#include "ffnx/graph/commands/AddVertex.h"
#include "ffnx/graph/commands/AddEdge.h"

namespace ffnx::ui::graph {

    /**
     * Provides Suggestions/Commands that can be applied based on a given selection state.
     */
    template <typename TGraph>
    class GraphCommandFactory {
    private:

        using Command = typename ffnx::graph::GraphCommand<TGraph>;

        std::weak_ptr<GraphSelectionModel<TGraph>> selection_model;

    public:

        GraphCommandFactory(std::weak_ptr<GraphSelectionModel<TGraph>> selection_model) :
            selection_model(selection_model) {
        }

        std::string get_command_suggestion() {
            // rules:
            // vertices / edges selected: offer to delete


            // nothing selected: offer to add vertex

            // two vertices selected: offer to add edge

            auto selection = selection_model.lock();

            if (selection->selected_edges().size() == 0) {
                if (selection->selected_vertices().size() == 0) {
                    return "Add Vertex";
                } else if (selection->selected_vertices().size() == 2) {
                    return "Add Edge";
                } else if (selection->selected_vertices().size() > 2) {
                    return "Add Edges";
                }
            }

            return "Remove";
        }


        void get_commands(std::vector<std::shared_ptr<Command>>& commands) {
            auto selection = selection_model.lock();

            if (selection->selected_edges().size() == 0) {
                if (selection->selected_vertices().size() == 0) {
                    commands.emplace_back(std::make_shared<ffnx::graph::commands::AddVertexCommand<TGraph>>());
                    return;
                } else if (selection->selected_vertices().size() >= 2) {
                    for (const auto& v0 : selection->selected_vertices()) {
                        for (const auto& v1 : selection->selected_vertices()) {
                            if (v0 != v1) {
                                commands.emplace_back(std::make_shared<ffnx::graph::commands::AddEdgeCommand<TGraph>>(v0, v1));
                            }
                        }
                    }

                    return;
                }
            }

            throw std::runtime_error("Not Implemented");
        }

    };
}

#endif