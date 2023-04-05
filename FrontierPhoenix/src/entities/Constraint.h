#ifndef FRONTIER_PHOENIX_CONSTRAINT
#define FRONTIER_PHOENIX_CONSTRAINT

#include "Shape.h"

#include <set>
#include <memory>

namespace ffnx::entities {

    struct Constraint {
    public:
        std::string identifier;
        std::set<std::shared_ptr<Shape>> associated_shapes;

        explicit Constraint(std::string identifier) : identifier(std::move(identifier)) {

        }
    };
}


#endif