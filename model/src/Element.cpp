#include "cwapi3d/model/Element.h"

namespace CwAPI3D::Model {

Element::Element(std::string name,
								 std::optional<std::string> externalElementId,
								 std::optional<std::string> externalGuid)
		: ModelEntity(std::move(name)),
			mExternalElementId(std::move(externalElementId)),
			mExternalGuid(std::move(externalGuid)) {}

}  // namespace CwAPI3D::Model
