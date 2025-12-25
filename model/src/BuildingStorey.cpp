#include "cwapi3d/model/BuildingStorey.h"

namespace CwAPI3D::Model {

BuildingStorey::BuildingStorey(std::string name, std::optional<double> elevation)
    : ModelEntity(std::move(name)), mElevation(elevation) {}

}  // namespace CwAPI3D::Model
