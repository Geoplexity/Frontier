#ifndef FRONTIER_PHOENIX_HYPERGRAPH_ANNOTATOR_H
#define FRONTIER_PHOENIX_HYPERGRAPH_ANNOTATOR_H

#include <boost/bimap.hpp>
#include "hypergraph/EventDispatching.h"
#include "hypergraph/Sparse.h"

namespace ffnx::hypergraph {

    template <typename TNodeAnnotation, typename TEdgeAnnotation>
    class Annotator : public HyperGraphListener {
    private:
        typedef boost::bimap<std::shared_ptr<HyperNode>, TNodeAnnotation> node_map;
        node_map nodeAnnotations;

        typedef boost::bimap<std::shared_ptr<HyperEdge>, TEdgeAnnotation> edge_map;
        edge_map edgeAnnotations;

    public:
        void nodeRemoved(node_ptr node) override {
            nodeAnnotations.left.erase(node);
        }

        void edgeRemoved(edge_ptr edge) override {
            edgeAnnotations.left.erase(edge);
        }

        void associate(std::shared_ptr<HyperNode> node, TNodeAnnotation value) {
            typename decltype(nodeAnnotations)::value_type vt(node, value);
            nodeAnnotations.insert(vt);
        }

        void associate(std::shared_ptr<HyperEdge> edge, TEdgeAnnotation value) {
            typename decltype(edgeAnnotations)::value_type vt(edge, value);
            edgeAnnotations.insert(vt);
        }

        TNodeAnnotation getAnnotation(node_ptr node) {
            return nodeAnnotations.left.at(node);
        }

        TEdgeAnnotation getAnnotation(edge_ptr edge) {
            return edgeAnnotations.left.at(edge);
        }
    };

    template <typename TNodeAnnotation, typename TEdgeAnnotation>
    class AnnotatedHypergraph {
    private:
        std::shared_ptr<HyperGraph> _graph;
        std::shared_ptr<Annotator<TNodeAnnotation, TEdgeAnnotation>> _annotator;

        AnnotatedHypergraph(std::shared_ptr<HyperGraph> graph,
                            std::shared_ptr<Annotator<TNodeAnnotation, TEdgeAnnotation>> annotator) :
                _graph(std::move(graph)), _annotator(annotator) {

        }

    public:

        HyperGraph& graph() {
            return *_graph;
        }

        Annotator<TNodeAnnotation, TEdgeAnnotation>& annotator() {
            return *_annotator;
        }

        void visitNodes(std::function<void(node_ptr, TNodeAnnotation)> visitor) const {
            _graph->visitNodes([&](auto node){
                visitor(node, this->_annotator->getAnnotation(node));
            });
        }

        void visitEdges(std::function<void(edge_ptr, TEdgeAnnotation)> visitor) const {
            _graph->visitEdges([&](auto edge){
                visitor(edge, this->_annotator->getAnnotation(edge));
            });
        }

        void visitEdgeNodes(const edge_ptr_ext& edge,
                            std::function<void(node_ptr, TNodeAnnotation)> visitor) const {
            _graph->visitEdgeNodes(edge, [&](auto node){
                visitor(node, this->_annotator->getAnnotation(node));
            });
        }

        /**
         * Adds a node and associates it with the given annotation.
         */
        std::shared_ptr<HyperNode> addNode(TNodeAnnotation nodeAnnotation) {
            auto node = _graph->addNode().lock();

            _annotator->associate(node, nodeAnnotation);

            return node;
        }

        /**
         * Adds an edge and associates it with the given annotation.
         */
        std::shared_ptr<HyperEdge> addEdge(TEdgeAnnotation edgeAnnotation) {
            auto edge = _graph->addEdge().lock();

            _annotator->associate(edge, edgeAnnotation);

            return edge;
        }

        static AnnotatedHypergraph<TNodeAnnotation, TEdgeAnnotation> create(std::shared_ptr<HyperGraph> base) {
            auto hg = std::make_shared<EventDispatchingHyperGraph>(base);
            auto annotator = std::make_shared<Annotator<TNodeAnnotation, TEdgeAnnotation>>();
            hg->addListener(annotator);

            return AnnotatedHypergraph(hg, annotator);
        }

        static AnnotatedHypergraph<TNodeAnnotation, TEdgeAnnotation> create() {
            return create(std::make_shared<SparseHyperGraph>());
        }
    };
}

#endif