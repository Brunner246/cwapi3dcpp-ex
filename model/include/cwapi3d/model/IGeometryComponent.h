#pragma once

#include <array>
#include <memory>
#include <string>

#include "cwapi3d/model/IComponent.h"
#include "cwapi3d/model/cwapi3d_model_export.h"

namespace CwAPI3D::Model {

/**
 * @brief Interface for geometry components
 * 
 * Geometry is injected into elements, not owned by them.
 * This allows:
 * - Multiple geometry representations (BRep, mesh, parametric)
 * - LOD (Level of Detail) switching
 * - Lazy loading / streaming
 * - Shared geometry instances
 */
class CWAPI3D_MODEL_EXPORT IGeometryComponent : public IComponent {
   public:
    ~IGeometryComponent() override = default;

    [[nodiscard]] std::string componentType() const override { return "IGeometryComponent"; }

    // Bounding box in local coordinates
    struct BoundingBox {
        std::array<double, 3> min;
        std::array<double, 3> max;
    };

    [[nodiscard]] virtual BoundingBox getBoundingBox() const = 0;
    [[nodiscard]] virtual std::string getGeometryType() const = 0;
};

}  // namespace CwAPI3D::Model
