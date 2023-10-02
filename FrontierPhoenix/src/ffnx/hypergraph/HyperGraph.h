#ifndef FRONTIER_PHOENIX_HYPERGRAPH_H
#define FRONTIER_PHOENIX_HYPERGRAPH_H

#include <set>
#include <memory>
#include <exception>
#include <functional>
#include <boost/bimap.hpp>
#include <utility>

/**
 * Interface for a generic hypergraph. Implementation may be pointer-based, sparse/dense matrix etc. Weak pointers are
 * used to uniquely identify node/edges. Nodes and edges themselves provide nothing more than identifier information.
 *
 * Mapping between nodes/edges and more interesting types is handled by annotation.
 */
namespace ffnx::hypergraph {

    class HyperNode {
    public:
        virtual ~HyperNode() = default;
    };

    class HyperEdge {
    public:
        virtual ~HyperEdge() = default;
    };

    using node_ptr_ext = std::weak_ptr<HyperNode>;
    using edge_ptr_ext = std::weak_ptr<HyperEdge>;

    using node_ptr = std::shared_ptr<HyperNode>;
    using edge_ptr = std::shared_ptr<HyperEdge>;

    class HyperGraph {
    public:
        [[nodiscard]] virtual bool containsNode(node_ptr_ext node) const = 0;

        [[nodiscard]] virtual bool containsEdge(edge_ptr_ext edge) const = 0;

        virtual void visitNodes(const std::function<void (node_ptr)> &visitor) const = 0;
        virtual void visitEdges(const std::function<void (edge_ptr)> &visitor) const = 0;
        virtual void visitEdgeNodes(edge_ptr_ext edge,
                                    const std::function<void (node_ptr)> &visitor) const = 0;

        virtual node_ptr_ext addNode() = 0;

        virtual void removeNode(node_ptr_ext toRemove) = 0;

        virtual edge_ptr_ext addEdge() = 0;

        virtual void removeEdge(std::weak_ptr<HyperEdge> toRemove) = 0;

        virtual void connect(node_ptr_ext node, edge_ptr_ext edge) = 0;

        virtual void disconnect(node_ptr_ext node, edge_ptr_ext edge) = 0;
    };


}

#endif