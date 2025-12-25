#pragma once

#include <optional>
#include <string>

#include "cwapi3d/model/ModelEntity.h"

namespace CwAPI3D::Model {

/**
 * @brief Element represents a stable identity for any building component
 * 
 * Design principles:
 * - Element is JUST an identity - no geometry, no attributes
 * - Geometry and attributes are injected via ComponentRegistry
 * - Relations (floor assignment) are stored separately
 * - Supports runtime modification without breaking references
 * - Suitable for Undo/Redo operations
 */
class CWAPI3D_MODEL_EXPORT Element : public ModelEntity {
   public:
    explicit Element(std::string name = "Element",
                    std::optional<std::string> externalElementId = std::nullopt,
                    std::optional<std::string> externalGuid = std::nullopt);

    [[nodiscard]] std::string_view kind() const noexcept override { return "Element"; }

    [[nodiscard]] const std::optional<std::string>& externalElementId() const noexcept {
        return mExternalElementId;
    }
    void setExternalElementId(std::string externalId) {
        mExternalElementId = std::move(externalId);
    }
    void clearExternalElementId() { mExternalElementId.reset(); }

    [[nodiscard]] const std::optional<std::string>& externalGuid() const noexcept {
        return mExternalGuid;
    }
    void setExternalGuid(std::string guid) { mExternalGuid = std::move(guid); }
    void clearExternalGuid() { mExternalGuid.reset(); }

   private:
    std::optional<std::string> mExternalElementId;
    std::optional<std::string> mExternalGuid;
};

}  // namespace CwAPI3D::Model
