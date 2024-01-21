#ifndef FFNX_UI_GRAPH_GRAPHSELECTIONMODEL
#define FFNX_UI_GRAPH_GRAPHSELECTIONMODEL

#include "ffnx/graph/Interface.h"

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

    public:

        const std::set<vert_desc>& selected_vertices() const {
            return _selected_vertices;
        }

        const std::set<edge_desc>& selected_edges() const {
            return _selected_edges;
        }

        bool is_selection_empty() const {
            return _selected_vertices.size() == 0 && _selected_edges.size() == 0;
        }

        void select_vertex(const vert_desc& vert) {
            if (_selected_vertices.contains(vert)) {
                throw std::runtime_error("Vertex is already selected");
            }

            _selected_vertices.insert(vert);
        }

        void deselect_vertex(const vert_desc& vert) {
            if (!_selected_vertices.contains(vert)) {
                throw std::runtime_error("Vertex is already deselected");
            }

            _selected_vertices.erase(vert);
        }


        void select_edge(const edge_desc& edge) {
            if (_selected_edges.contains(edge)) {
                throw std::runtime_error("Edge is already selected");
            }

            _selected_edges.insert(edge);
        }

        void deselect_edge(const edge_desc& edge) {
            if (!_selected_edges.contains(edge)) {
                throw std::runtime_error("Edge is already deselected");
            }

            _selected_edges.erase(edge);
        }

    };
}

#endif