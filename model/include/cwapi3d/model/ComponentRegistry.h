#pragma once

#include <memory>
#include <stdexcept>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include "cwapi3d/model/EntityId.h"
#include "cwapi3d/model/IComponent.h"
#include "cwapi3d/model/cwapi3d_model_export.h"

namespace CwAPI3D::Model {

/**
 * @brief Type-safe component storage and retrieval
 * 
 * The ComponentRegistry is the key to dependency injection in this architecture.
 * It allows runtime association of components with entities without modifying
 * the entity classes themselves.
 * 
 * Design principles:
 * - Type-safe: Uses std::type_index for component identification
 * - Multiple components: An entity can have many components of different types
 * - No ownership transfer: Registry owns components
 * - Query-friendly: Fast lookup by entity and component type
 * 
 * Example:
 *   registry.addComponent(element.id(), std::make_unique<BRepGeometry>(...));
 *   auto* geom = registry.getComponent<BRepGeometry>(element.id());
 */
class CWAPI3D_MODEL_EXPORT ComponentRegistry {
   public:
    ComponentRegistry() = default;
    ~ComponentRegistry() = default;

    // Non-copyable, movable
    ComponentRegistry(const ComponentRegistry&) = delete;
    ComponentRegistry& operator=(const ComponentRegistry&) = delete;
    ComponentRegistry(ComponentRegistry&&) = default;
    ComponentRegistry& operator=(ComponentRegistry&&) = default;

    /**
     * @brief Add a component to an entity
     * 
     * If an entity already has a component of this type, it will be replaced.
     * The registry takes ownership of the component.
     */
    template <typename T>
    void addComponent(const EntityId& entityId, std::unique_ptr<T> component) {
        if (!component) {
            throw std::invalid_argument("Cannot add null component");
        }

        const auto typeIndex = std::type_index(typeid(T));
        mComponents[entityId][typeIndex] = std::move(component);
    }

    /**
     * @brief Get a component from an entity
     * 
     * Returns nullptr if the entity doesn't have a component of this type.
     */
    template <typename T>
    [[nodiscard]] T* getComponent(const EntityId& entityId) const {
        auto entityIt = mComponents.find(entityId);
        if (entityIt == mComponents.end()) {
            return nullptr;
        }

        const auto typeIndex = std::type_index(typeid(T));
        auto componentIt = entityIt->second.find(typeIndex);
        if (componentIt == entityIt->second.end()) {
            return nullptr;
        }

        return dynamic_cast<T*>(componentIt->second.get());
    }

    /**
     * @brief Check if an entity has a component of the given type
     */
    template <typename T>
    [[nodiscard]] bool hasComponent(const EntityId& entityId) const {
        return getComponent<T>(entityId) != nullptr;
    }

    /**
     * @brief Remove a component from an entity
     * 
     * Returns true if the component was removed, false if it didn't exist.
     */
    template <typename T>
    bool removeComponent(const EntityId& entityId) {
        auto entityIt = mComponents.find(entityId);
        if (entityIt == mComponents.end()) {
            return false;
        }

        const auto typeIndex = std::type_index(typeid(T));
        return entityIt->second.erase(typeIndex) > 0;
    }

    /**
     * @brief Remove all components from an entity
     */
    void removeAllComponents(const EntityId& entityId);

    /**
     * @brief Get all entities that have a specific component type
     */
    template <typename T>
    [[nodiscard]] std::vector<EntityId> getEntitiesWithComponent() const {
        std::vector<EntityId> result;
        const auto typeIndex = std::type_index(typeid(T));

        for (const auto& [entityId, components] : mComponents) {
            if (components.contains(typeIndex)) {
                result.push_back(entityId);
            }
        }

        return result;
    }

    /**
     * @brief Get the number of components attached to an entity
     */
    [[nodiscard]] size_t getComponentCount(const EntityId& entityId) const;

    /**
     * @brief Get the total number of entities with components
     */
    [[nodiscard]] size_t getEntityCount() const noexcept { return mComponents.size(); }

   private:
    // EntityId -> (TypeIndex -> Component)
    std::unordered_map<EntityId, std::unordered_map<std::type_index, std::unique_ptr<IComponent>>>
        mComponents;
};

}  // namespace CwAPI3D::Model
