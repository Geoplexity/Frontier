#ifndef FRONTIER_PHOENIX_FLOWGRAPH_INTERFACE_H
#define FRONTIER_PHOENIX_FLOWGRAPH_INTERFACE_H

#include "FlowGraph.h"
#include "Command.h"

namespace ffnx::flowgraph {

    template <typename TVertDataType, typename TEdgeDataType>
    class FlowGraphInterface {
    private:
        FlowGraph<TVertDataType, TEdgeDataType> flowGraph;

        using cmd_ptr = std::shared_ptr<FlowGraphCommand<TVertDataType, TEdgeDataType>>;

        std::vector<cmd_ptr> commandStack;
        std::set<cmd_ptr> commandSet;

    public:

        /**
         * @return const ref to graph for interrogation.
         */
        const FlowGraph<TVertDataType, TEdgeDataType>& graph() {
            return flowGraph;
        }

        /**
         * Apply the specified command to the flow graph and move it onto the command stack.
         * Note that duplicate commands are not allowed.
         */
        template <typename TCmd>
        TCmd applyCommand(TCmd cmd) {
            auto abs_ptr = std::static_pointer_cast<FlowGraphCommand<TVertDataType, TEdgeDataType>>(cmd);

            if (commandSet.contains(abs_ptr)) {
                throw std::runtime_error("Command has already been applied to this stack.");
            }

            cmd->apply(flowGraph);
            commandStack.push_back(abs_ptr);
            commandSet.insert(abs_ptr);

            return cmd;
        }

        /**
         * Undo the last command in the stack and return its pointer.
         */
        cmd_ptr undoCommand() {
            if (commandStack.empty()) {
                throw std::runtime_error("No commands left to undo.");
            }

            commandStack.back()->undo(flowGraph);

            cmd_ptr result = commandStack.back();
            commandStack.pop_back();
            commandSet.erase(result);

            return result;
        }

    };

}

#endif