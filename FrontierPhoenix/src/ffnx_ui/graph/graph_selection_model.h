#ifndef FFNX_UI_GRAPH_GRAPHSELECTIONMODEL
#define FFNX_UI_GRAPH_GRAPHSELECTIONMODEL

#include "ffnx/graph/Interface.h"
#include "ffnx/event/observer.h"

namespace ffnx::ui::graph {

    /**
     * Selection model for a graph.
     */
    template <typename TGraph>
    class GraphSelectionModel {
    private:

        using vert_desc = typename TGraph::vertex_descriptor;
        using edge_desc = typename TGraph::edge_descriptor;

        std::set<vert_desc> _selected_vertices;
        std::set<edge_desc> _selected_edges;

        ffnx::event::Subject<> _selection_changed_subject;

    public:

        ffnx::event::Subject<>& selection_changed_subject() {
            return _selection_changed_subject;
        }

        const std::set<vert_desc>& selected_vertices() {
            return _selected_vertices;
        }

        const std::set<edge_desc>& selected_edges() {
            return _selected_edges;
        }

        void select_vertex(const vert_desc& vert) {
            if (_selected_vertices.contains(vert)) {
                throw std::runtime_error("Vertex is already selected");
            }

            _selected_vertices.insert(vert);

            _selection_changed_subject.notify();
        }

        void deselect_vertex(const vert_desc& vert) {
            if (!_selected_vertices.contains(vert)) {
                throw std::runtime_error("Vertex is already deselected");
            }

            _selected_vertices.erase(vert);

            _selection_changed_subject.notify();
        }


        void select_edge(const edge_desc& edge) {
            if (_selected_edges.contains(edge)) {
                throw std::runtime_error("Edge is already selected");
            }

            _selected_edges.insert(edge);

            _selection_changed_subject.notify();
        }

        void deselect_edge(const edge_desc& edge) {
            if (!_selected_edges.contains(edge)) {
                throw std::runtime_error("Edge is already deselected");
            }

            _selected_edges.erase(edge);

            _selection_changed_subject.notify();
        }

    };
}

#endif