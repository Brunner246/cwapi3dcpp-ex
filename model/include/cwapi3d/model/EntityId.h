#pragma once

#include <compare>
#include <cstdint>
#include <string>

#include "cwapi3d/model/cwapi3d_model_export.h"

namespace CwAPI3D::Model {
/**
 * @brief Strong type for entity identifiers
 *
 * Provides a stable, unique identity for all domain entities.
 * Thread-safe generation via atomic counter.
 */
class CWAPI3D_MODEL_EXPORT EntityId {
   public:
    // Create a new unique ID
    static EntityId create();

    // Create an invalid ID (for default construction)
    EntityId() : mId(0) {}

    // Explicit construction from uint64_t (use with caution)
    explicit EntityId(uint64_t id) : mId(id) {}

    [[nodiscard]] bool isValid() const noexcept { return mId != 0; }
    [[nodiscard]] uint64_t value() const noexcept { return mId; }
    [[nodiscard]] std::string toString() const;

    bool operator==(const EntityId& other) const noexcept { return mId == other.mId; }
    bool operator!=(const EntityId& other) const noexcept { return mId != other.mId; }
    bool operator<(const EntityId& other) const noexcept { return mId < other.mId; }

   private:
    uint64_t mId;
};

}  // namespace CwAPI3D::Model

// Hash support for std::unordered_map
namespace std {
template <>
struct hash<CwAPI3D::Model::EntityId> {
    size_t operator()(const CwAPI3D::Model::EntityId& id) const noexcept {
        return std::hash<uint64_t>{}(id.value());
    }
};
}  // namespace std
