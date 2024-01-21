#ifndef FFNX_UI_GRAPH_UI_UI_COMMAND_CONSOLIDATOR_H
#define FFNX_UI_GRAPH_UI_UI_COMMAND_CONSOLIDATOR_H

#include "ffnx/graph/Command.h"
#include "ui_command.h"

namespace ffnx::ui::graph {

    /**
     * Combines ui commands that are redundant. e.g. two consecutive vertex move commands can be consolidated by
     * removing the initial move.
     */
    template <typename TGraph>
    class UICommandConsolidator {


    public:
        /**
         * Checks the most recently added ui graph command to see if any previous ui command can be removed
         * as a consolidation. e.g. if a vertex is selected, some other ui operations are performed, and the
         * same vertex is then deselected, the initial selection command can be removed.
         */
        void consolidate(std::vector<std::unique_ptr<UIGraphCommand<TGraph>>>& command_stack, int pending_command_index) {
            auto it = command_stack.rbegin();

            auto last_command_category = command_stack[pending_command_index - 1]->ui_command().command_category();
            if (!last_command_category->permits_consolidation) {
                return;
            }

            for (int i = pending_command_index - 1; i >= 0 ; i--) {
                if (!command_stack[i]->is_ui_command()) {
                    // only UI commands can be consolidated
                    return;
                }

                auto previous_command_category = command_stack[i]->ui_command().command_category();
                if (!previous_command_category->permits_consolidation) {
                    return;
                }

                if (*previous_command_category == *last_command_category) {
                    std::cout << "Erasing command category from command stack: " << last_command_category->identifier << std::endl;
                    command_stack.erase(command_stack.begin() + i);
                    return;
                }
            }
        }

    };


};

#endif