#ifndef FRONTIER_PHOENIX_PEBBLEGAME_PEBBLEGAMEMOVE
#define FRONTIER_PHOENIX_PEBBLEGAME_PEBBLEGAMEMOVE

#include <vector>
#include <memory>

namespace ffnx::pebblegame {

    template <typename VertDesc, typename EdgeDesc>
    class PebbleGameMove {
    public:

        /**
         * Types of possible moves. Each type corresponds to a value class. E.g. STARTED corresponds to StartedMove,
         * etc.
         */
        enum Type {
            /**
             * Event fired before any other game moves are made.
             */
            STARTED,

            /**
             * A new edge of the input graph is being considered. No further actions are taken this move.
             */
            INPUT_EDGE_CONSIDERATION,

            /**
             * An edge of the input graph that has been considered is rejected.
             */
            INPUT_EDGE_REJECTION,

            /**
             * Indicates an edge-add move was performed. e.g. from i -> j with a pebble removed from i
             */
            EDGE_ADDED,

            /**
             * A depth-first-search of the existing game graph has started, or is in progress.
             */
            PEBBLE_SEARCH_DFS,

            /**
             * Indicates that an existing edge (e.g. i -> j) is reversed, resulting in j -> i, with a pebble moved to i
             */
            EDGE_REVERSED,

            /**
             * Final event fired. No further events are fired after this one.
             */
            FINISHED
        };

        /**
         * Indicates that the game graph is in the initial state.
         */
        struct StartedMove {

        };

        struct InputEdgeConsiderationMove {
            VertDesc v0;
            VertDesc v1;
        };

        struct InputEdgeRejectionMove {
            VertDesc v0;
            VertDesc v1;
        };

        struct EdgeAddedMove {
            VertDesc v0;
            VertDesc v1;
        };

        struct PebbleSearchDFSMove {

            /**
             * The stack currently representing the state of the dfs. Last element is top of stack.
             */
            std::vector<VertDesc> dfs_vertices;
            std::vector<EdgeDesc> dfs_edges;

            PebbleSearchDFSMove() : dfs_vertices(), dfs_edges() {
            }
        };

        struct EdgeReversedMove {

            /**
             * First vertex AFTER this move was performed (previously v1)
             */
            VertDesc v0;

            /**
             * Second vertex AFTER this move was performed (previously v0)
             */
            VertDesc v1;
        };

        /**
         * Game finished. All edges have been considered. No further moves will be made. (unless a new game is started)
         */
        struct FinishedMove {

        };

    private:

        union {
            StartedMove _started_move;
            InputEdgeConsiderationMove _input_edge_consideration_move;
            InputEdgeRejectionMove _input_edge_rejection_move;
            EdgeAddedMove _edge_added_move;
            PebbleSearchDFSMove _pebble_search_dfs_move;
            EdgeReversedMove _edge_reversed_move;
            FinishedMove _finished_move;
        };


    public:
        std::optional<Type> type;

        PebbleGameMove(): type(std::nullopt) {
        }

        ~PebbleGameMove() {
            if (!type.has_value()) {
                return;
            }

            switch(type.value()) {
                case STARTED:
                    _started_move.~StartedMove();
                    break;
                case INPUT_EDGE_CONSIDERATION:
                    _input_edge_consideration_move.~InputEdgeConsiderationMove();
                    break;
                case INPUT_EDGE_REJECTION:
                    _input_edge_rejection_move.~InputEdgeRejectionMove();
                    break;
                case EDGE_ADDED:
                    _edge_added_move.~EdgeAddedMove();
                    break;
                case PEBBLE_SEARCH_DFS:
                    _pebble_search_dfs_move.~PebbleSearchDFSMove();
                    break;
                case EDGE_REVERSED:
                    _edge_reversed_move.~EdgeReversedMove();
                    break;
                case FINISHED:
                    _finished_move.~FinishedMove();
                    break;
            }
        }

        const StartedMove& as_started_move() const {
            assert_of_type(STARTED);
            return _started_move;
        }

        const InputEdgeConsiderationMove& as_input_edge_consideration_move() const {
            assert_of_type(INPUT_EDGE_CONSIDERATION);
            return _input_edge_consideration_move;
        }

        const InputEdgeRejectionMove& as_input_edge_rejection_move() const {
            assert_of_type(INPUT_EDGE_REJECTION);
            return _input_edge_rejection_move;
        }

        const EdgeAddedMove& as_edge_added_move() const {
            assert_of_type(EDGE_ADDED);
            return _edge_added_move;
        }

        const PebbleSearchDFSMove& as_pebble_search_dfs_move() const {
            assert_of_type(PEBBLE_SEARCH_DFS);
            return _pebble_search_dfs_move;
        }

        const EdgeReversedMove& as_edge_reversed_move() const {
            assert_of_type(EDGE_REVERSED);
            return _edge_reversed_move;
        }

        const FinishedMove& as_finished_move() const {
            assert_of_type(FINISHED);
            return _finished_move;
        }

        static std::unique_ptr<PebbleGameMove<VertDesc, EdgeDesc>> started() {
            auto result = std::make_unique<PebbleGameMove<VertDesc, EdgeDesc>>();
            result->type = STARTED;
            new (&result->_started_move) StartedMove();
            return std::move(result);
        }

        static std::unique_ptr<PebbleGameMove<VertDesc, EdgeDesc>>
        input_edge_consideration(const VertDesc& v0, const VertDesc& v1) {
            auto result = std::make_unique<PebbleGameMove<VertDesc, EdgeDesc>>();
            result->type = INPUT_EDGE_CONSIDERATION;
            new (&result->_input_edge_consideration_move) InputEdgeConsiderationMove();
            result->_input_edge_consideration_move.v0 = v0;
            result->_input_edge_consideration_move.v1 = v1;
            return std::move(result);
        }

        static std::unique_ptr<PebbleGameMove<VertDesc, EdgeDesc>>
        input_edge_rejection(const VertDesc& v0, const VertDesc& v1) {
            auto result = std::make_unique<PebbleGameMove<VertDesc, EdgeDesc>>();
            result->type = INPUT_EDGE_REJECTION;
            new (&result->_input_edge_rejection_move) InputEdgeRejectionMove();
            result->_input_edge_rejection_move.v0 = v0;
            result->_input_edge_rejection_move.v1 = v1;
            return std::move(result);
        }

        static std::unique_ptr<PebbleGameMove<VertDesc, EdgeDesc>>
        edge_added(const VertDesc& v0, const VertDesc& v1) {
            auto result = std::make_unique<PebbleGameMove<VertDesc, EdgeDesc>>();
            result->type = EDGE_ADDED;
            new (&result->_edge_added_move) EdgeAddedMove();
            result->_edge_added_move.v0 = v0;
            result->_edge_added_move.v1 = v1;
            return std::move(result);
        }

        static std::unique_ptr<PebbleGameMove<VertDesc, EdgeDesc>>
        edge_reversed(const VertDesc& v0, const VertDesc& v1) {
            auto result = std::make_unique<PebbleGameMove<VertDesc, EdgeDesc>>();
            result->type = EDGE_REVERSED;
            new (&result->_edge_reversed_move) EdgeReversedMove();
            result->_edge_reversed_move.v0 = v0;
            result->_edge_reversed_move.v1 = v1;
            return std::move(result);
        }

        static std::unique_ptr<PebbleGameMove<VertDesc, EdgeDesc>>
        pebble_search_dfs(const std::vector<VertDesc> &v_stack, const std::vector<EdgeDesc> &e_stack) {
            auto result = std::make_unique<PebbleGameMove<VertDesc, EdgeDesc>>();
            result->type = PEBBLE_SEARCH_DFS;
            new (&result->_pebble_search_dfs_move) PebbleSearchDFSMove();
            result->_pebble_search_dfs_move.dfs_vertices = v_stack;
            result->_pebble_search_dfs_move.dfs_edges = e_stack;
            return std::move(result);
        }

        static std::unique_ptr<PebbleGameMove<VertDesc, EdgeDesc>> finished() {
            auto result = std::make_unique<PebbleGameMove<VertDesc, EdgeDesc>>();
            result->type = FINISHED;
            new (&result->_finished_move) FinishedMove();
            return std::move(result);
        }


    private:

        void assert_of_type(const Type& type) const {
            if (!this->type.has_value() || this->type.value() != type) {
                throw std::runtime_error("PebbleGameMove type not as expected");
            }
        }

    };

}

#endif
