#include <gtest/gtest.h>

#include "sketcher/sketcher.h"
#include "mapping/hyper_to_flow/SketchMapper.h"

using namespace ffnx;


TEST(HyperToFlowTests, testEmpty)
{
    sketcher::SketcherBuilder builder;

    auto sketcher = builder.buildSketcher();

    auto hypergraph = sketcher.getSketchHyperGraph();

    auto flowgraph = mapping::SketchMapper::hyperToFlow(hypergraph);
}

TEST(HyperToFlowTests, testComplex)
{
    sketcher::SketcherBuilder builder;
    builder.registerShapeType("point_2d", {2});
    builder.registerShapeType("line_2d", {4});

    builder.registerConstraintType({"horizontal", {"point_2d", "point_2d"}}, { {}, {
            "abs(shape0.y - shape1.y)"
    } });
    builder.registerConstraintType({"horizontal", {"line_2d"}}, { {}, {
            "abs(shape0.y0 - shape0.y1)"
    } });

    builder.registerConstraintType({"vertical", {"point_2d", "point_2d"}}, { {}, {
            "abs(shape0.x - shape1.x)"
    } });
    builder.registerConstraintType({"vertical", {"line_2d"}}, { {}, {
            "abs(shape0.x0 - shape0.x1)"
    } });

    builder.registerConstraintType({"distance", {"point_2d", "point_2d"}}, { { sketcher::FLOAT }, {
            "hypot(shape0.x - shape1.x, shape0.y - shape1.y) - arg0"
    } });

    auto sketcher = builder.buildSketcher();

    auto p0 = sketcher.addShape("point_2d", { 0, 0 });
    auto p1 = sketcher.addShape("point_2d", { 1, 1 });
    auto p2 = sketcher.addShape("point_2d", { 2, 1 });
    auto l0 = sketcher.addShape("line_2d", { 10, 10, 20, 20 });

    sketcher.addConstraint("horizontal", {p0, p1}, {});
    sketcher.addConstraint("vertical", {p0, p2}, {});

    sketcher.addConstraint("horizontal", { l0 }, {});

    auto hypergraph = sketcher.getSketchHyperGraph();

    auto flowgraph = mapping::SketchMapper::hyperToFlow(hypergraph);
}
