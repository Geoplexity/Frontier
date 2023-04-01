
#include <iostream>
#include <memory>

#include "entities/SketchMapper.h"

int main()
{
    auto s0 = std::make_shared<ffnx::entities::Shape>("line0");
    auto s1 = std::make_shared<ffnx::entities::Shape>("line1");
    auto s2 = std::make_shared<ffnx::entities::Shape>("line2");

    auto c0 = std::make_shared<ffnx::entities::Constraint>("tangency");
    c0->associated_shapes.insert(s0);
    c0->associated_shapes.insert(s1);

    auto c1 = std::make_shared<ffnx::entities::Constraint>("normalcy");
    c1->associated_shapes.insert(s1);
    c1->associated_shapes.insert(s2);

    ffnx::entities::Sketch sketch;
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