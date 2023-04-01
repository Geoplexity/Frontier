#ifndef FRONTIER_PHOENIX_SKETCH_H
#define FRONTIER_PHOENIX_SKETCH_H

#include "Shape.h"
#include "Constraint.h"

#include <set>
#include <memory>

namespace ffnx::entities {
        class Sketch {
        public:
            std::set<std::shared_ptr<Shape>> shapes;
            std::set<std::shared_ptr<Constraint>> constraints;
        };
    }
#endif
