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
        explicit EventDispatchingHyperGraph(std::shared_ptr<HyperGraph> delegate) : delegate(std::move(delegate)) {

        }

        [[nodiscard]] bool containsNode(std::weak_ptr<HyperNode> node) const override {
            return delegate->containsNode(node);
        }

        [[nodiscard]] bool containsEdge(std::weak_ptr<HyperEdge> edge) const override {
            return delegate->containsEdge(edge);
        }

        void addListener(const std::shared_ptr<HyperGraphListener>& listener) {
            if (listeners.contains(listener)) {
                throw std::runtime_error("Listener already registered");
            }

            listeners.insert(listener);
        }

        void removeListener(const std::shared_ptr<HyperGraphListener>& listener) {
            if (!listeners.contains(listener)) {
                throw std::runtime_error("Listener cannot be removed: not registered");
            }

            listeners.erase(listener);
        }

        void visitEdges(const std::function<void (std::shared_ptr<HyperEdge>)> &visitor) const override {
            delegate->visitEdges(visitor);
        }

        void visitNodes(const std::function<void (std::shared_ptr<HyperNode>)> &visitor) const override {
            delegate->visitNodes(visitor);
        }

        void visitEdgeNodes(std::weak_ptr<HyperEdge> edge,
                            const std::function<void (std::shared_ptr<HyperNode>)> &visitor) const override {
            delegate->visitEdgeNodes(edge, visitor);
        }

        std::weak_ptr<HyperNode> addNode() override {
            auto result = delegate->addNode().lock();

            notify([&](auto listener){ listener->nodeAdded(result); });

            return result;
        }

        void removeNode(std::weak_ptr<HyperNode> toRemove) override {
            // removed node must be preserved as shared ptr, otherwise will be deleted before listeners are notified
            auto toRemovePtr = toRemove.lock();

            delegate->removeNode(toRemove);

            notify([&](auto listener){ listener->nodeRemoved(toRemovePtr); });
        }

        std::weak_ptr<HyperEdge> addEdge() override {
            std::shared_ptr<HyperEdge> result = delegate->addEdge().lock();

            notify([&](auto listener){ listener->edgeAdded(result); });

            return result;
        }

        void removeEdge(std::weak_ptr<HyperEdge> toRemove) override {
            // removed node must be preserved as shared ptr, otherwise will be deleted before listeners are notified
            auto toRemovePtr = toRemove.lock();

            delegate->removeEdge(toRemove);

            notify([&](auto listener){ listener->edgeRemoved(toRemovePtr); });
        }

        void connect(std::weak_ptr<HyperNode> node,
                     std::weak_ptr<HyperEdge> edge) override {
            auto nodePtr = node.lock();
            auto edgePtr = edge.lock();

            delegate->connect(node, edge);

            notify([&](auto listener){ listener->connected(nodePtr, edgePtr); });
        }

        void disconnect(std::weak_ptr<HyperNode> node,
                        std::weak_ptr<HyperEdge> edge) override {
            auto nodePtr = node.lock();
            auto edgePtr = edge.lock();

            delegate->disconnect(node, edge);

            notify([&](auto listener){ listener->disconnected(nodePtr, edgePtr); });
        }

    private:
        void notify(const std::function<void(std::shared_ptr<HyperGraphListener>)>& notifier) {
            for (auto& listener : listeners) {
                notifier(listener);
            }
        }
    };
}

#endif