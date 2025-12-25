#pragma once

#include <optional>
#include <string>

#include "cwapi3d/model/ModelEntity.h"

namespace CwAPI3D::Model {

/**
 * @brief Floor (Storey) represents a horizontal organizational level
 * 
 * Design principles:
 * - Floor does NOT contain elements
 * - Element-to-Floor relations are stored in RelationRegistry
 * - Allows runtime reassignment without object restructuring
 * - Elevation is optional (can be computed from geometry if needed)
 */
class CWAPI3D_MODEL_EXPORT BuildingStorey : public ModelEntity {
   public:
    explicit BuildingStorey(std::string name = "Floor", std::optional<double> elevation = std::nullopt);

    [[nodiscard]] std::string_view kind() const noexcept override { return "Floor"; }

    [[nodiscard]] const std::optional<double>& elevation() const noexcept { return mElevation; }
    void setElevation(double elevation) { mElevation = elevation; }
    void clearElevation() { mElevation.reset(); }

   private:
    std::optional<double> mElevation;
};

}  // namespace CwAPI3D::Model
