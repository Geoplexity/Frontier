#ifndef FRONTIER_PHOENIX_DRPLAN_PLANS_DRPLAN
#define FRONTIER_PHOENIX_DRPLAN_PLANS_DRPLAN

#include <set>
#include "cluster/Cluster.h"
#include "tree/TreeNode.h"

namespace ffnx::drplan {

    /**
     * Single Hierarchical decomposition-recomposition plan.
     * The plan is represented as a tree of nested clusters, containing the
     * vertices to be solved independently. The root node of the tree should be the solution root.
     */
    template <typename TVert, typename TEdge>
    class DRPlan {

    private:

        /**
         * The graph that the solution is applied to.
         */
        std::shared_ptr<const ffnx::flowgraph::FlowGraph<TVert, TEdge>> graph;

        std::shared_ptr<ffnx::tree::TreeNode<
                ffnx::cluster::Cluster<TVert, TEdge>>> _solution_tree;

    public:

        explicit DRPlan(
                std::shared_ptr<ffnx::tree::TreeNode<ffnx::cluster::Cluster<TVert, TEdge>>> solution_tree) : _solution_tree(solution_tree) {

        }

        const ffnx::tree::TreeNode<ffnx::cluster::Cluster<TVert, TEdge>>& solution_tree() {
            return *_solution_tree;
        }

        class Builder {
        private:

            using FlowGraph = flowgraph::FlowGraph<TVert, TEdge>;

            using graph_t = flowgraph::FlowGraph<TVert, TEdge>;
            using cluster_t = ffnx::cluster::Cluster<TVert, TEdge>;

            using graph_ptr = std::weak_ptr<const graph_t>;
            using cluster_ptr = std::shared_ptr<cluster_t>;

            using tree_node_t = ffnx::tree::TreeNode<cluster_t>;
            using tree_node_ptr = std::shared_ptr<tree_node_t>;
            using tree_node_wk_ptr = std::weak_ptr<tree_node_t>;

            graph_ptr graph;

            bool has_built = false;

            tree_node_ptr result;

            // result nodes are cached to easily validate if input is from the result tree hierarchy and not a different
            // tree.
            std::set<tree_node_ptr> result_nodes;

        public:

            explicit Builder(graph_ptr graph) :
                graph(graph),
                result(std::make_shared<tree_node_t>(*cluster_t::Builder::of_graph(graph))) {
            }

            tree_node_wk_ptr root() const {
                return result;
            }

            tree_node_wk_ptr add_branch(tree_node_wk_ptr parent_node, const cluster_t& cluster) {
                auto parent_node_ptr = parent_node.lock();

                //if (!result_nodes.contains(parent_node_ptr)) {
                //    throw std::runtime_error("Attempt to add TreeNode instance which does not derive from root node.");
                //}

                if (parent_node_ptr->value().is_equivalent(cluster)) {
                    throw std::runtime_error("Attempt to add duplicate cluster");
                }

                if (!parent_node_ptr->value().includes(cluster)) {
                    throw std::runtime_error("Cluster is not a subset of parent node");
                }

                auto new_node_ptr = parent_node_ptr->append_child_node(cluster).lock();

                result_nodes.insert(new_node_ptr);

                return new_node_ptr;
            }

            DRPlan build() {
                assert_not_built();

                return DRPlan(result);
            }

        private:
            void assert_not_built() {
                if (has_built) {
                    throw std::runtime_error("Build already called");
                }
            }
        };
    };
};

#endif