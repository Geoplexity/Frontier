#ifndef FRONTIER_PHOENIX_SHAPE
#define FRONTIER_PHOENIX_SHAPE

#include <string>

namespace ffnx::entities {
    struct Shape {
        std::string label;

    public:
        explicit Shape(std::string label) : label(std::move(label)) {

        }
    };
}

#endif