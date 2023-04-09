#ifndef FRONTIER_PHOENIX_HYPERGRAPH_EVENTDISPATCHINGHYPERGRAPH_H
#define FRONTIER_PHOENIX_HYPERGRAPH_EVENTDISPATCHINGHYPERGRAPH_H

#include "hypergraph/HyperGraph.h"

namespace ffnx::hypergraph {

    class HyperGraphListener {
    public:
        virtual void nodeAdded(std::shared_ptr<HyperNode> node) { }
        virtual void nodeRemoved(std::shared_ptr<HyperNode> node) { }

        virtual void edgeAdded(std::shared_ptr<HyperEdge> node) { }
        virtual void edgeRemoved(std::shared_ptr<HyperEdge> node) { }

        virtual void connected(std::shared_ptr<HyperNode> node, std::shared_ptr<HyperEdge> edge) { }
        virtual void disconnected(std::shared_ptr<HyperNode> node, std::shared_ptr<HyperEdge> edge) { }
    };

    class EventDispatchingHyperGraph : public HyperGraph {
    private:
        std::set<std::shared_ptr<HyperGraphListener>> listeners;

        std::shared_ptr<HyperGraph> delegate;

    public:
        explicit EventDispatchingHyperGraph(std::shared_ptr<HyperGraph> delegate);

        [[nodiscard]] bool containsNode(std::weak_ptr<HyperNode> node) const override;

        [[nodiscard]] bool containsEdge(std::weak_ptr<HyperEdge> edge) const override;

        void addListener(const std::shared_ptr<HyperGraphListener>& listener);

        void removeListener(const std::shared_ptr<HyperGraphListener>& listener);

        void visitEdges(const std::function<void (std::shared_ptr<HyperEdge>)> &visitor) const override;

        void visitNodes(const std::function<void (std::shared_ptr<HyperNode>)> &visitor) const override;

        void visitEdgeNodes(std::weak_ptr<HyperEdge> edge,
                            const std::function<void (std::shared_ptr<HyperNode>)> &visitor) const override;

        std::weak_ptr<HyperNode> addNode() override;

        void removeNode(std::weak_ptr<HyperNode> toRemove) override;

        std::weak_ptr<HyperEdge> addEdge() override;

        void removeEdge(std::weak_ptr<HyperEdge> toRemove) override;

        void connect(std::weak_ptr<HyperNode> node,
                     std::weak_ptr<HyperEdge> edge) override;

        void disconnect(std::weak_ptr<HyperNode> node,
                        std::weak_ptr<HyperEdge> edge) override;

    private:
        void notify(const std::function<void(std::shared_ptr<HyperGraphListener>)>& notifier);
    };
}

#endif