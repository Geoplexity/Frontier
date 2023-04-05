#include "sketcher.h"

#include <boost/algorithm/string.hpp>

void ffnx::sketcher::SketcherBuilder::register_shape_type(
        const shape_signature &signature,
        const ShapeSpecification &shapeSpecification) {

    if (this->shapeTypes.contains(signature)) {
        std::stringstream ss;
        ss << "Shape signature " << signature << " has already been registered";

        throw std::runtime_error(ss.str());
    }

    this->shapeTypes[signature] = shapeSpecification;
}

std::string format_constraint_signature(const ffnx::sketcher::constraint_signature& sig) {
    std::stringstream ss;
    ss << std::get<0>(sig) << "(" << boost::algorithm::join(std::get<1>(sig), ",") << ")";

    return ss.str();
}

void ffnx::sketcher::SketcherBuilder::register_constraint_type(
                                                            const constraint_signature &signature,
                                                            const ConstraintSpecification &constraintSpecification) {
    if (this->constraintTypes.contains(signature)) {
        std::stringstream ss;
        ss << "Constraint signature " << format_constraint_signature(signature) <<
            " has already been registered";

        throw std::runtime_error(ss.str());
    }

    this->constraintTypes[signature] = constraintSpecification;
}

ffnx::sketcher::Sketcher ffnx::sketcher::SketcherBuilder::build_sketcher() {
    return ffnx::sketcher::Sketcher(
            this->shapeTypes,
            this->constraintTypes);
}

ffnx::sketcher::Sketcher::Sketcher(const std::map<shape_signature, ShapeSpecification> &shapeTypes,
                                   const std::map<constraint_signature, ConstraintSpecification> &constraintTypes) :
                                   shapeTypes(shapeTypes),
                                   constraintTypes(constraintTypes) {

}

ffnx::sketcher::shape_ptr ffnx::sketcher::Sketcher::add_shape(
        const std::string &identifier,
        const std::vector<double>& inputValues) {

    if (!this->shapeTypes.contains(identifier)) {
        std::stringstream ss;
        ss << "Shape identifier " << identifier << " not known.";
        throw std::runtime_error(ss.str());
    }

    const auto expectedDof = this->shapeTypes[identifier].dof;
    if (inputValues.size() != expectedDof) {
        std::stringstream ss;
        ss << inputValues.size() << " input values specified but " << expectedDof << " expected for " << identifier;
        throw std::runtime_error(ss.str());
    }

    auto ptr = std::make_shared<ffnx::entities::Shape>(identifier, expectedDof);
    for (int i = 0; i < inputValues.size(); i++) {
        ptr->setParameter(i, inputValues[i]);
    }

    this->sketch.shapes.insert(ptr);
    return ptr;
}

std::weak_ptr<ffnx::entities::Constraint>
ffnx::sketcher::Sketcher::add_constraint(const std::string &identifier,
                                        const std::vector<shape_ptr> &shapes,
                                        const std::vector<ConstraintArgument> &arguments) {

    std::vector<shape_signature> shapeSigs;
    for (const auto& s : shapes) {
        shapeSigs.push_back(s.lock()->identifier);
    }

    const constraint_signature signature = {identifier, shapeSigs};
    if (!constraintTypes.contains(signature)) {
        throw std::runtime_error((std::stringstream() <<
            "Cannot create constraint with signature " << format_constraint_signature(signature) <<
            ". No such constraint has been specified.").str());
    }

    const ConstraintSpecification constraintSpecification = constraintTypes[signature];

    // todo: pass expression objects

    auto ptr = std::make_shared<ffnx::entities::Constraint>(
            format_constraint_signature(signature));

    for (const auto& s : shapes) {
        ptr->associated_shapes.insert(s.lock());
    }

    sketch.constraints.insert(ptr);

    return ptr;
}
