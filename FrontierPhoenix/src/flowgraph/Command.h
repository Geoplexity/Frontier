#ifndef FRONTIER_PHOENIX_FLOWGRAPH_COMMAND
#define FRONTIER_PHOENIX_FLOWGRAPH_COMMAND

#include "FlowGraph.h"

namespace ffnx::flowgraph {

    /**
     * Represents a single mutation of the flowgraph. For example flow redistribution.
     */
    template <typename TVertDataType, typename TEdgeDataType>
    class FlowGraphCommand {
    private:

        /**
         * Indicates whether the apply()-undo() cycle is complete.
         * true = an apply() called with no corresponding undo()
         * false = no calls, or n * (apply(), undo())
         */
        bool hasApplied = false;

    public:

        /**
         * Applies re-writing to graph according ot the command's purpose. e.g. if the command describes adding a node,
         * this function performs the operation via boost::add_vertex.
         */
        virtual void apply(FlowGraph<TVertDataType, TEdgeDataType> &flowGraph) final {
            if (hasApplied) {
                throw std::runtime_error("Apply already called.");
            }

            applyImpl(flowGraph);
            hasApplied = true;
        }

        /**
         * Undo any changes caused by apply().
         */
        virtual void undo(FlowGraph<TVertDataType, TEdgeDataType> &flowGraph) final {
            if (!hasApplied) {
                throw std::runtime_error("No changes to undo.");
            }

            undoImpl(flowGraph);
            hasApplied = false;
        }

        virtual ~FlowGraphCommand() = default;

    protected:
        /**
         * Apply and persist information regarding any changes. Implementers must store all information required to
         * "undo" the command. Implementers may assume that data from previous calls to applyImpl can be discarded.
         *
         * Implementers may throw runtime exceptions to indicate states where the command could not be carried out.
         */
        virtual void applyImpl(FlowGraph<TVertDataType, TEdgeDataType> &flowGraph) = 0;

        /**
         * Undo the changes caused by applyImpl(). Implementers may assume that applyImpl() has been called before this
         * function.
         *
         * As with applyImpl(), runtime exceptions may be thrown to indicate that the command could not be undone.
         */
        virtual void undoImpl(FlowGraph<TVertDataType, TEdgeDataType> &flowGraph) = 0;
    };

}

#endif