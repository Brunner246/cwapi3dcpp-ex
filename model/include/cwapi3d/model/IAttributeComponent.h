#pragma once

#include <any>
#include <memory>
#include <string>
#include <unordered_map>

#include "cwapi3d/model/IComponent.h"
#include "cwapi3d/model/cwapi3d_model_export.h"

namespace CwAPI3D::Model {

/**
 * @brief Interface for attribute components
 * 
 * Attributes are injected into elements, providing metadata,
 * properties, or any custom data without modifying Element class.
 * 
 * Examples:
 * - Material properties
 * - Cost data
 * - Classification codes
 * - Custom metadata
 */
class CWAPI3D_MODEL_EXPORT IAttributeComponent : public IComponent {
   public:
    ~IAttributeComponent() override = default;

    [[nodiscard]] std::string componentType() const override { return "IAttributeComponent"; }

    [[nodiscard]] virtual bool hasAttribute(const std::string& key) const = 0;
    [[nodiscard]] virtual std::any getAttribute(const std::string& key) const = 0;
    virtual void setAttribute(const std::string& key, std::any value) = 0;
    virtual void removeAttribute(const std::string& key) = 0;
};

}  // namespace CwAPI3D::Model
