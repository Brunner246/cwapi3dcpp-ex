#pragma once

#include <memory>
#include <string>
#include <typeindex>

namespace CwAPI3D::Model {

/**
 * @brief Base interface for all components
 * 
 * Components are injected into elements at runtime via ComponentRegistry.
 * This allows elements to have geometry, attributes, or any custom data
 * without tight coupling or inheritance hierarchies.
 * 
 * Design principles:
 * - Interface-based: All components implement this base
 * - Type-safe: Components identified by std::type_index
 * - Multiple components: An element can have many components
 * - Runtime injection: No compile-time dependencies
 */
class IComponent {
   public:
    virtual ~IComponent() = default;

    [[nodiscard]] virtual std::string componentType() const = 0;

    // Optional: Clone support for copy operations
    [[nodiscard]] virtual std::unique_ptr<IComponent> clone() const = 0;
};

}  // namespace CwAPI3D::Model
