#include "cwapi3d/model/ComponentRegistry.h"

namespace CwAPI3D::Model {

void ComponentRegistry::removeAllComponents(const EntityId& entityId) {
    mComponents.erase(entityId);
}

size_t ComponentRegistry::getComponentCount(const EntityId& entityId) const {
    auto it = mComponents.find(entityId);
    if (it == mComponents.end()) {
        return 0;
    }
    return it->second.size();
}

}  // namespace CwAPI3D::Model
