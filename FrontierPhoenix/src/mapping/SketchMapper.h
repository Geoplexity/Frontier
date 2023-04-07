#ifndef FRONTIER_PHOENIX_SKETCH_SKETCH_MAPPER_H
#define FRONTIER_PHOENIX_SKETCH_SKETCH_MAPPER_H

#include <boost/bimap.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <memory>

#include "mapping/MappedGraph.h"
#include "hypergraph/HyperGraph.h"
#include "hypergraph/Annotator.h"

namespace ffnx::mapping {

    using FlowGraph = ffnx::flowgraph::FlowGraph;

    using sketch_hypergraph = ffnx::hypergraph::AnnotatedHypergraph<
            std::shared_ptr<Shape>,
            std::shared_ptr<Constraint>>;

    class SketchMapper {
    public:
        static std::unique_ptr<MappedGraph> createMappedGraph(const sketch_hypergraph &annotated_sketch) {
            std::unique_ptr<MappedGraph> result = std::make_unique<MappedGraph>();

            auto source_vert = boost::add_vertex(result->graph());
            result->graph()[source_vert].label = "source";
            auto sink_vert = boost::add_vertex(result->graph());
            result->graph()[sink_vert].label = "sink";

            annotated_sketch.visitNodes([&](auto node, auto shape){
                const auto flow_vert = boost::add_vertex(result->graph());
                result->graph()[flow_vert].label = shape->identifier;
                result->set_pair(shape, flow_vert);

                // connects up the flow from shape to sink
                boost::add_edge(flow_vert, sink_vert, result->graph());
            });

            annotated_sketch.visitEdges([&](auto edge, auto constraint){
                const auto constraint_flow_vert = boost::add_vertex(result->graph());
                result->graph()[constraint_flow_vert].label = constraint->identifier;
                result->set_pair(constraint, constraint_flow_vert);

                boost::add_edge(source_vert, constraint_flow_vert, result->graph());

                // constraints must be connected to all shapes
                annotated_sketch.visitEdgeNodes(edge, [&](auto node, auto shape){
                    const auto shape_flow_vert = result->vert_for_shape(shape);
                    boost::add_edge(constraint_flow_vert, shape_flow_vert, result->graph());
                });

            });


            return std::move(result);
        }

    };

}

#endif