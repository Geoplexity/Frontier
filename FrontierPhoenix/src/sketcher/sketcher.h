#ifndef FRONTIER_PHOENIX_SKETCHER_H
#define FRONTIER_PHOENIX_SKETCHER_H

/**
 * Define an abstract sketching interface.
 */
namespace ffnx::sketcher {

    class SketcherInterface {
    public:

        /**
         * Core relation is placing geometric objects and constraints between them.
         *
         *  To start, Shape classes must be specified. The rendering aspect is not regarded here, so they can be
         *  described solely in terms of their degrees of freedom (any reason why this might not be the case..?):
         *
         *  sketcher->registerShapeClass(2, "point_2d")
         *  sketcher->registerShapeClass({ "point_2d", "point_2d" }, "line_2d") // composite shape shorthand?
         *
         *  Then constraint classes may be registered:
         *
         *  // to register a constraint implementation, we must specify how the constraint restricts the degrees of
         *  // freedom. This boils down to generating an expression in terms of the dof points that is == 0 (a sln).
         *  sketcher->registerConstraintClass("incident", "point_2d", "point_2d", {
         *      p0.x - p1.x == 0
         *      p0.y - p1.y == 0
         *  })
         *
         *  sketcher->registerConstraintClass("vertical", "point_2d", "point_2d", {
         *      p0.x - p1.x == 0
         *  })
         *  sketcher->registerConstraintClass("horizontal", "point_2d", "point_2d", {
         *      p0.y - p1.y == 0
         *  })
         *
         *  sketcher->registerConstraintImplementation("tangent", "line_2d", "line_2d", {
         *      l0 end is tangent to l1 start ? (maybe extra args needed for variety)
         *
         *      sketcher->constraintEqn("incident", l0.p1, l1.p0)
         *
         *      // slopes must be equal, need to deal with h lines etc..
         *      slope of l0 = (l0.p0.x - l0.p1.x) / (l0.p0.y - l0.p1.y)
         *      slope of l1 = ...
         *
         *      sl0 - sl1 == 0
         *  })
         *
         *
         *  sketcher->registerConstraintImplementation("distance", "int", "point_2d", "point_2d", {
         *      auto dist_val = constraint_args[0] // better way to formulate this?
         *
         *      pow(p0.x - p1.x, 2) + pow(p0.y - p1.y) - dist_val * dist_val == 0
         *  })
         *
         *  Once shape and constraint types have been specified, the sketch can then be built up:
         *
         *  auto p0 = sketcher->createShape("point_2d")
         *  auto p1 = sketcher->createShape("point_2d")
         *  auto p2 = sketcher->createShape("point_2d")
         *  sketcher->createConstraint("vertical", p0, p1)
         *  sketcher->createConstraint("horizontal", p0, p1)
         *  sketcher->createConstraint("distance", p2, p1, 1) // etc...
         *
         *  sketcher->getFlowGraph() // <-- returns the flow graph, which may then be processed via DecompRecomp
         */

    };

}



#endif