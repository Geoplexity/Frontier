#ifndef FRONTIER_PHOENIX_MAPPED_GRAPH_H
#define FRONTIER_PHOENIX_MAPPED_GRAPH_H

#include "Shape.h"
#include "Constraint.h"
#include "Graph.h"
#include <boost/bimap.hpp>

namespace ffnx::entities {

    using m_ptr = std::shared_ptr<MappingToken>;

    /**
     * Using the mapping tokens, the corresponding shapes can be referred back to.
     */
    struct MappedGraph {
        std::unique_ptr<FlowGraph> _graph = std::make_unique<FlowGraph>();

        boost::bimap<m_ptr, std::shared_ptr<Shape>> shapes;

        boost::bimap<m_ptr, std::shared_ptr<Constraint>> constraints;

        boost::bimap<m_ptr, FlowGraph::vertex_descriptor> vertices;

        [[nodiscard]] FlowGraph &graph() const {
            return *_graph;
        }

    public:

        void set_pair(const std::shared_ptr<Shape> &s, const FlowGraph::vertex_descriptor &vd) {
            const m_ptr token = std::make_shared<MappingToken>();

            vertices.insert(decltype(vertices)::value_type(token, vd));
            shapes.insert(decltype(shapes)::value_type(token, s));
        }

        void set_pair(const std::shared_ptr<Constraint> &c, const FlowGraph::vertex_descriptor &vd) {
            const m_ptr token = std::make_shared<MappingToken>();
            vertices.insert(decltype(vertices)::value_type(token, vd));
            constraints.insert(decltype(constraints)::value_type(token, c));
        }

        std::shared_ptr<Shape> shape_for_vert(const FlowGraph::vertex_descriptor &vd) {
            const auto token = vertices.right.at(vd);
            return shapes.left.at(token);
        }

        std::shared_ptr<Constraint> constraint_for_vert(const FlowGraph::vertex_descriptor &vd) {
            const auto token = vertices.right.at(vd);
            return constraints.left.at(token);
        }

        FlowGraph::vertex_descriptor vert_for_constraint(const std::shared_ptr<Constraint> &c) {
            const auto token = constraints.right.at(c);
            return vertices.left.at(token);
        }

        FlowGraph::vertex_descriptor vert_for_shape(const std::shared_ptr<Shape> &s) {
            const auto token = shapes.right.at(s);
            return vertices.left.at(token);
        }
    };
}

#endif