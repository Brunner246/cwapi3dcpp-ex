#include "cwapi3d/composite/ComponentVisitor.h"
#include "cwapi3d/composite/Component.h"
#include <iostream>

namespace cwapi3d::composite {

void PrintVisitor::visitComposite(const Composite& composite) const {

    mOs << "+ " << composite.name() << " (Composite, "
         << composite.childCount() << " children)\n";

    for (const auto& child : composite) {
        child->accept(*this);
    }
}

void PrintVisitor::visitLeaf(const Leaf& leaf) const {

    mOs << "- " << leaf.name() << " (Leaf)\n";
}

} // namespace cwapi3d::composite
