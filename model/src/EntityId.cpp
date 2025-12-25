#include "cwapi3d/model/EntityId.h"

#include <atomic>
#include <sstream>

namespace CwAPI3D::Model {

EntityId EntityId::create() {
    static std::atomic<uint64_t> counter{1};
    return EntityId{counter.fetch_add(1, std::memory_order_relaxed)};
}

std::string EntityId::toString() const {
    std::ostringstream oss;
    oss << "EntityId(" << mId << ")";
    return oss.str();
}

}  // namespace CwAPI3D::Model
