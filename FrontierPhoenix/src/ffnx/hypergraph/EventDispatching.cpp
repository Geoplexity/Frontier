#include "EventDispatching.h"

namespace ffnx::hypergraph {

    EventDispatchingHyperGraph::EventDispatchingHyperGraph(std::shared_ptr<HyperGraph> delegate) : delegate(std::move(delegate)) {

    }

    bool EventDispatchingHyperGraph::containsNode(std::weak_ptr<HyperNode> node) const {
        return delegate->containsNode(node);
    }

    bool EventDispatchingHyperGraph::containsEdge(std::weak_ptr<HyperEdge> edge) const {
        return delegate->containsEdge(edge);
    }

    void EventDispatchingHyperGraph::addListener(const std::shared_ptr<HyperGraphListener> &listener) {
        if (listeners.contains(listener)) {
            throw std::runtime_error("Listener already registered");
        }

        listeners.insert(listener);
    }

    void EventDispatchingHyperGraph::removeListener(const std::shared_ptr<HyperGraphListener> &listener) {
        if (!listeners.contains(listener)) {
            throw std::runtime_error("Listener cannot be removed: not registered");
        }

        listeners.erase(listener);
    }

    void EventDispatchingHyperGraph::visitEdges(const std::function<void(std::shared_ptr<HyperEdge>)> &visitor) const {
        delegate->visitEdges(visitor);
    }

    void EventDispatchingHyperGraph::visitNodes(const std::function<void(std::shared_ptr<HyperNode>)> &visitor) const {
        delegate->visitNodes(visitor);
    }

    void EventDispatchingHyperGraph::visitEdgeNodes(std::weak_ptr<HyperEdge> edge, const std::function<void(
            std::shared_ptr<HyperNode>)> &visitor) const {
        delegate->visitEdgeNodes(edge, visitor);
    }

    std::weak_ptr<HyperNode> EventDispatchingHyperGraph::addNode() {
        auto result = delegate->addNode().lock();

        notify([&](auto listener){ listener->nodeAdded(result); });

        return result;
    }

    void EventDispatchingHyperGraph::removeNode(std::weak_ptr<HyperNode> toRemove) {
        // removed node must be preserved as shared ptr, otherwise will be deleted before listeners are notified
        auto toRemovePtr = toRemove.lock();

        delegate->removeNode(toRemove);

        notify([&](auto listener){ listener->nodeRemoved(toRemovePtr); });
    }

    std::weak_ptr<HyperEdge> EventDispatchingHyperGraph::addEdge() {
        std::shared_ptr<HyperEdge> result = delegate->addEdge().lock();

        notify([&](auto listener){ listener->edgeAdded(result); });

        return result;
    }

    void EventDispatchingHyperGraph::removeEdge(std::weak_ptr<HyperEdge> toRemove) {
        // removed node must be preserved as shared ptr, otherwise will be deleted before listeners are notified
        auto toRemovePtr = toRemove.lock();

        delegate->removeEdge(toRemove);

        notify([&](auto listener){ listener->edgeRemoved(toRemovePtr); });
    }

    void EventDispatchingHyperGraph::connect(std::weak_ptr<HyperNode> node, std::weak_ptr<HyperEdge> edge) {
        auto nodePtr = node.lock();
        auto edgePtr = edge.lock();

        delegate->connect(node, edge);

        notify([&](auto listener){ listener->connected(nodePtr, edgePtr); });
    }

    void EventDispatchingHyperGraph::disconnect(std::weak_ptr<HyperNode> node, std::weak_ptr<HyperEdge> edge) {
        auto nodePtr = node.lock();
        auto edgePtr = edge.lock();

        delegate->disconnect(node, edge);

        notify([&](auto listener){ listener->disconnected(nodePtr, edgePtr); });
    }

    void EventDispatchingHyperGraph::notify(const std::function<void(std::shared_ptr<HyperGraphListener>)> &notifier) {
        for (auto& listener : listeners) {
            notifier(listener);
        }
    }
}