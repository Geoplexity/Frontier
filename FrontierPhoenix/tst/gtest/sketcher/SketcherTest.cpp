#include <gtest/gtest.h>

#include "ffnx/sketcher/sketcher.h"

using namespace ffnx;

TEST(SketcherTests, testInit)
{
    sketcher::SketcherBuilder builder;

    auto sketcher = builder.buildSketcher();

    auto hypergraph = sketcher.getSketchHyperGraph();
}

TEST(SketcherTests, testRegisterSingleShape)
{
    sketcher::SketcherBuilder builder;

    builder.registerShapeType("point2d", { 2 });

    auto sketcher = builder.buildSketcher();

    sketcher.addShape("point2d", {1, 2});
}

TEST(SketcherTests, testUnregisteredShape)
{
    sketcher::SketcherBuilder builder;

    builder.registerShapeType("point2d", { 2 });

    auto sketcher = builder.buildSketcher();

    ASSERT_THROW(
            sketcher.addShape("notregistered", {1, 2}),
            std::runtime_error);

}

TEST(SketcherTests, testInvalidArgs)
{
    sketcher::SketcherBuilder builder;

    builder.registerShapeType("point2d", { 2 });

    auto sketcher = builder.buildSketcher();

    ASSERT_THROW(
            sketcher.addShape("point2d", {1}),
            std::runtime_error);

}

TEST(SketcherTests, testInvalidConstraintRegistration)
{
    sketcher::SketcherBuilder builder;

    ASSERT_THROW(
            builder.registerConstraintType(
                    { "constraint", { "not_existing" } },
                    {
                            {},
                            { "eqn" }
                    }),
            std::runtime_error);
}

TEST(SketcherTests, testDuplicateShapeSignature)
{
    sketcher::SketcherBuilder builder;

    builder.registerShapeType("point2d", {2});

    ASSERT_THROW(
            builder.registerShapeType("point2d", {2}),
            std::runtime_error);
}

TEST(SketcherTests, testDuplicateConstraintSignature)
{
    sketcher::SketcherBuilder builder;

    builder.registerShapeType("point2d", {2});

    builder.registerConstraintType({"constraint", {}}, { {}, {"eqn"} });

    ASSERT_THROW(
            builder.registerConstraintType({"constraint", {}}, { {}, {"eqn"} }),
            std::runtime_error);
}
