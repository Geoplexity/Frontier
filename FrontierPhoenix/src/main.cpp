
#include <iostream>
#include <memory>

#include "entities/SketchMapper.h"
#include "sketcher/sketcher.h"

namespace gnac = GiNaC;

int main1() {
    ffnx::sketcher::SketcherBuilder builder;
    builder.register_shape_type("point_2d", {2});

    builder.register_constraint_type(
            {"distance", {"point_2d", "point_2d"}},
            {
                { ffnx::sketcher::ConstraintArgumentType::FLOAT },
                { "hypot(s0.x - s1.x, s0.y - s1.y) - a0" }
            });

    builder.register_constraint_type(
            {"horizontal", {"point_2d", "point_2d"}},
            {
                    { },
                    { "abs(s0.y - s1.y)" }
            });

    builder.register_constraint_type(
            {"vertical", {"point_2d", "point_2d"}},
            {
                    { },
                    { "abs(s0.x - s1.x)" }
            });

    ffnx::sketcher::Sketcher sketcher = builder.build_sketcher();

    auto s0 = sketcher.add_shape("point_2d", {0, 0});
    auto s1 = sketcher.add_shape("point_2d", {10, 10});
    auto s2 = sketcher.add_shape("point_2d", {-10, 20});
    auto s3 = sketcher.add_shape("point_2d", {10, 20});

    sketcher.add_constraint("distance", {s0, s1}, {});
    sketcher.add_constraint("horizontal", {s1, s2}, {});
    sketcher.add_constraint("distance", {s0, s3}, {});
    sketcher.add_constraint("vertical", {s2, s3}, {});


    auto mapped_graph = std::move(ffnx::entities::SketchMapper::create_graph(sketcher.get_sketch_hypergraph()));

    std::ofstream file_out("./output.dot");

    boost::write_graphviz(file_out,
                          mapped_graph->graph(),
                          boost::make_label_writer(
                                  boost::get(&ffnx::entities::VertexData::label, mapped_graph->graph())));
    file_out.close();

    return EXIT_SUCCESS;
}


int main() {
    auto s0 = std::make_shared<ffnx::entities::Shape>("line0", 4);
    auto s1 = std::make_shared<ffnx::entities::Shape>("line1", 4);
    auto s2 = std::make_shared<ffnx::entities::Shape>("line2", 4);

    auto c0 = std::make_shared<ffnx::entities::Constraint>("tangency");
    c0->associated_shapes.insert(s0);
    c0->associated_shapes.insert(s1);

    auto c1 = std::make_shared<ffnx::entities::Constraint>("normalcy");
    c1->associated_shapes.insert(s1);
    c1->associated_shapes.insert(s2);

    ffnx::entities::SketchHypergraph sketch;
    sketch.shapes.insert(s0);
    sketch.shapes.insert(s1);
    sketch.shapes.insert(s2);

    sketch.constraints.insert(c0);
    sketch.constraints.insert(c1);

    auto mapped_graph = std::move(ffnx::entities::SketchMapper::create_graph(sketch));

    print_graph(mapped_graph->graph());

    std::ofstream file_out("graphviz_examples/output.dot");

    boost::write_graphviz(file_out,
                          mapped_graph->graph(),
                          boost::make_label_writer(
                                  boost::get(&ffnx::entities::VertexData::label, mapped_graph->graph())));
    file_out.close();

    return EXIT_SUCCESS;
}
