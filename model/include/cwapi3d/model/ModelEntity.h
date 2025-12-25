#pragma once

#include <optional>
#include <string>
#include <string_view>

#include "cwapi3d/model/EntityId.h"
#include "cwapi3d/model/cwapi3d_model_export.h"

namespace CwAPI3D::Model {

/**
 * @brief Base class for identifiable, named model entities
 *
 * Holds the stable identity (GUID) and common metadata so that derived
 * entities stay focused on their own responsibilities.
 */
class CWAPI3D_MODEL_EXPORT ModelEntity {
   public:
    explicit ModelEntity(std::string name, std::optional<std::string> description = std::nullopt)
        : mId(EntityId::create()), mName(std::move(name)), mDescription(std::move(description)) {}
    virtual ~ModelEntity() = default;

    [[nodiscard]] const EntityId& id() const noexcept { return mId; }
    [[nodiscard]] const std::string& name() const noexcept { return mName; }
    void setName(std::string name) { mName = std::move(name); }

    [[nodiscard]] const std::optional<std::string>& description() const noexcept {
        return mDescription;
    }
    void setDescription(std::string description) { mDescription = std::move(description); }
    void clearDescription() { mDescription.reset(); }

    [[nodiscard]] virtual std::string_view kind() const noexcept = 0;

   protected:
    EntityId mId;
    std::string mName;
    std::optional<std::string> mDescription;
};

}  // namespace CwAPI3D::Model
