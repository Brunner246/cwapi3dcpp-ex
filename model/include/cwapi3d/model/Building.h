#pragma once

#include <optional>
#include <string>

#include "cwapi3d/model/ModelEntity.h"

namespace CwAPI3D::Model {

/**
 * @brief Building represents the top-level organizational container
 * 
 * Design principles:
 * - Building does NOT contain floors or elements
 * - Floor-to-Building relations are stored in RelationRegistry
 * - Supports multi-building scenarios
 * - Each building can have optional site location data
 */
class CWAPI3D_MODEL_EXPORT Building : public ModelEntity {
   public:
    explicit Building(std::string name = "Building");

    [[nodiscard]] std::string_view kind() const noexcept override { return "Building"; }

    [[nodiscard]] const std::optional<std::string>& address() const noexcept { return mAddress; }
    void setAddress(std::string address) { mAddress = std::move(address); }
    void clearAddress() { mAddress.reset(); }

   private:
    std::optional<std::string> mAddress;
};

}  // namespace CwAPI3D::Model
