#ifndef FRONTIER_PHOENIX_SHAPE
#define FRONTIER_PHOENIX_SHAPE

#include <string>
#include <utility>
#include <vector>
#include <stdexcept>
#include <sstream>

namespace ffnx::entities {

    struct Shape {

        /**
         * Degrees of freedom associated with shape. e.g. Point2D==2, linesegment2d==4 etc.
         */
        int dof;

        /**
         * Describes the "type" of shape, e.g. circle, circle-arc, line-segment, point etc..
         */
        std::string identifier;

        /**
         * Values associated with DOF. TODO: should this be stored here? Use defined or generic instead of double?
         */
        std::vector<double> values;

        explicit Shape(
                std::string identifier,
                const int& dof) :
                       identifier(std::move(identifier)),
                       dof(dof) {

            for (int i = 0; i < dof; i++) {
                values.push_back(0);
            }
        }

        void setParameter(const int& index, const double& value) {
            if (index < 0 || index >= dof) {
                throw std::runtime_error((
                        std::stringstream() << "Index " << index << " out of range (shape has " <<
                        dof << " dof)").str()
                        );
            }

            values[index] = value;
        }
    };
}

#endif