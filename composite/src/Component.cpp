#include "cwapi3d/composite/Component.h"
#include "cwapi3d/composite/ComponentVisitor.h"
#include <algorithm>
#include <stdexcept>

namespace cwapi3d::composite {

// Composite implementation

void Composite::addChild(std::unique_ptr<Component>&& child) {
    if (!child) {
        throw std::invalid_argument("Cannot add null child");
    }
    
    if (child.get() == this) {
        throw std::invalid_argument("Cannot add component as its own child");
    }
    
    // Check if child is an ancestor (would create cycle)
    const Component* ancestor = this->parent();
    while (ancestor) {
        if (ancestor == child.get()) {
            throw std::invalid_argument("Cannot add ancestor as child (would create cycle)");
        }
        ancestor = ancestor->parent();
    }
    
    child->setParent(this);
    mChildren.push_back(std::move(child));
}

std::unique_ptr<Component> Composite::removeChild(Component* child) {
    if (!child) {
        return nullptr;
    }

    const auto it = std::ranges::find_if(
        mChildren,
        [child](const std::unique_ptr<Component>& c) {
            return c.get() == child;
        });
    
    if (it == mChildren.end()) {
        return nullptr;
    }
    
    auto removed = std::move(*it);
    removed->setParent(nullptr);
    mChildren.erase(it);
    
    return removed;
}

std::unique_ptr<Component> Composite::removeChild(const std::string& name) {
    const auto it = std::ranges::find_if(
        mChildren,
        [&name](const std::unique_ptr<Component>& c) {
            return c->name() == name;
        });
    
    if (it == mChildren.end()) {
        return nullptr;
    }
    
    auto removed = std::move(*it);
    removed->setParent(nullptr);
    mChildren.erase(it);
    
    return removed;
}

void Composite::clearChildren() {
    for (const auto& child : mChildren) {
        child->setParent(nullptr);
    }
    mChildren.clear();
}

Component* Composite::getChild(const size_t index) const {
    if (index >= mChildren.size()) {
        throw std::out_of_range("Child index out of range");
    }
    return mChildren[index].get();
}

Component* Composite::findChild(const std::string& name) const {
    const auto it = std::ranges::find_if(
        mChildren,
        [&name](const std::unique_ptr<Component>& c) {
            return c->name() == name;
        });
    
    return it != mChildren.end() ? it->get() : nullptr;
}

void Composite::traverse(const std::function<void(Component&)>& func) {
    func(*this);
    for (auto& child : mChildren) {
        if (child->isComposite()) {
            dynamic_cast<Composite*>(child.get())->traverse(func);
        } else {
            func(*child);
        }
    }
}

void Composite::traverse(const std::function<void(const Component&)>& func) const {
    func(*this);
    for (const auto& child : mChildren) {
        if (child->isComposite()) {
            dynamic_cast<const Composite*>(child.get())->traverse(func);
        } else {
            func(*child);
        }
    }
}

void Composite::accept(const ComponentVisitor& visitor) const {
    visitor.visitComposite(*this);
}

void Leaf::accept(const ComponentVisitor& visitor) const {
    visitor.visitLeaf(*this);
}

} // namespace cwapi3d::composite
