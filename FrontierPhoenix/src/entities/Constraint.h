#ifndef FRONTIER_PHOENIX_CONSTRAINT
#define FRONTIER_PHOENIX_CONSTRAINT

#include "Shape.h"

#include <set>
#include <memory>

namespace ffnx::entities {

    struct Constraint {
    public:
        std::string label;
        std::set<std::shared_ptr<Shape>> associated_shapes;

        explicit Constraint(std::string label) : label(std::move(label)) {

        }
    };
}


#endif