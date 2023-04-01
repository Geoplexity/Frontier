#ifndef FRONTIER_PHOENIX_SKETCH_MAPPER_H
#define FRONTIER_PHOENIX_SKETCH_MAPPER_H

#include "Sketch.h"
#include "MappedGraph.h"
#include <boost/bimap.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <memory>

namespace ffnx::entities {

    class SketchMapper {
    public:
        static std::unique_ptr<MappedGraph> create_graph(const Sketch &sketch) {
            std::unique_ptr<MappedGraph> result = std::make_unique<MappedGraph>();

            auto source_vert = boost::add_vertex(result->graph());
            result->graph()[source_vert].label = "source";
            auto sink_vert = boost::add_vertex(result->graph());
            result->graph()[sink_vert].label = "sink";

            for (const auto &s: sketch.shapes) {
                const auto vert = boost::add_vertex(result->graph());
                result->graph()[vert].label = s->label;
                result->set_pair(s, vert);

                boost::add_edge(vert, sink_vert, result->graph());
            }

            for (const auto &c: sketch.constraints) {
                const auto vert = boost::add_vertex(result->graph());
                result->graph()[vert].label = c->label;
                result->set_pair(c, vert);

                boost::add_edge(source_vert, vert, result->graph());
            }

            // source -> cons -> shapes -> sink
            for (const auto &c: sketch.constraints) {
                const auto constraint_vert = result->vert_for_constraint(c);
                for (const auto &s: c->associated_shapes) {
                    const auto shape_vert = result->vert_for_shape(s);

                    boost::add_edge(constraint_vert, shape_vert, result->graph());
                }
            }

            return std::move(result);
        }

    };

}

#endif