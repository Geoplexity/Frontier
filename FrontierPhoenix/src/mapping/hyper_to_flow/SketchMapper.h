#ifndef FRONTIER_PHOENIX_SKETCH_SKETCH_MAPPER_H
#define FRONTIER_PHOENIX_SKETCH_SKETCH_MAPPER_H

#include <boost/bimap.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <memory>
#include <utility>

#include "graph/Interface.h"
#include "graph/commands/AddVertex.h"
#include "graph/commands/AddEdge.h"

#include "hypergraph/HyperGraph.h"
#include "hypergraph/Annotator.h"

#include "sketcher/Shape.h"
#include "sketcher/Constraint.h"


namespace ffnx::mapping {

    using shape_ptr = std::weak_ptr<ffnx::sketcher::Shape>;
    using constraint_ptr = std::weak_ptr<ffnx::sketcher::Constraint>;

    struct SketchReference {
        shape_ptr shape;
        constraint_ptr constraint;
        std::string label;
    };

    using FlowGraph = ffnx::graph::FlowGraph<SketchReference, SketchReference>;
    using FlowGraphInterface = ffnx::graph::FlowGraphInterface<SketchReference, SketchReference>;

    using map_out = std::unique_ptr<FlowGraphInterface>;
    using map_out_vd = FlowGraph::vertex_descriptor;
    using map_out_ed = FlowGraph::edge_descriptor;

    using add_vert_cmd = ffnx::graph::commands::AddVertexCommand<FlowGraph>;
    using add_edge_cmd = ffnx::graph::commands::AddEdgeCommand<FlowGraph>;

    using map_in = ffnx::hypergraph::AnnotatedHypergraph<
            std::shared_ptr<ffnx::sketcher::Shape>,
            std::shared_ptr<ffnx::sketcher::Constraint>>;

    class SketchMapper {
    public:
        static map_out hyperToFlow(const map_in &sketch) {
            map_out result = std::make_unique<FlowGraphInterface>();

            auto source_vd = result->applyCommand(std::make_shared<add_vert_cmd>([](SketchReference& ref){
                ref.label = "source";
            }))->getVertex();

            auto sink_vd = result->applyCommand(std::make_shared<add_vert_cmd>([](SketchReference& ref){
                ref.label = "sink";
            }))->getVertex();

            std::map<
                    std::shared_ptr<ffnx::sketcher::Shape>,
                    map_out_vd> mappedShapes;

            sketch.visitNodes([&](auto node, auto shape){
                auto vd = result->applyCommand(std::make_shared<add_vert_cmd>([shape](SketchReference& ref){
                    ref.label = shape->identifier;
                    ref.shape = shape;
                }))->getVertex();

                result->applyCommand(std::make_shared<add_edge_cmd>(vd, sink_vd));

                mappedShapes[shape] = vd;
            });

            sketch.visitEdges([&](auto edge, auto constraint){
                auto constraint_vd = result->applyCommand(std::make_shared<add_vert_cmd>([constraint](SketchReference& ref){
                    ref.label = constraint->identifier;
                    ref.constraint = constraint;
                }))->getVertex();

                result->applyCommand(std::make_shared<add_edge_cmd>(source_vd, constraint_vd));

                sketch.visitEdgeNodes(edge, [&](auto node, auto shape){
                    auto shape_vd = mappedShapes[shape];

                    result->applyCommand(std::make_shared<add_edge_cmd>(constraint_vd, shape_vd));
                });

            });

            return std::move(result);
        }

    };

}

#endif