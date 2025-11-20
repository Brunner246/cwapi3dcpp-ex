#pragma once

#include "cwapi3d/composite/Component.h"
#include <iostream>

// Forward declare to avoid circular dependency in this section
namespace CwAPI3D::Composite {
template<typename T> class ComponentBase;
template<typename T> class CompositeNode;
template<typename T> class LeafNode;
}

namespace CwAPI3D::Composite {

// Forward declarations for non-generic types
class Component;
class Composite;
class Leaf;

// Template classes are forward declared in Component.h
// template<typename T> class ComponentBase;
// template<typename T> class CompositeNode;
// template<typename T> class LeafNode;

/**
 * @brief Generic visitor interface for traversing the component hierarchy
 * @tparam T The data type stored in the components
 */
template<typename T>
class ComponentVisitorBase {
public:
    virtual ~ComponentVisitorBase() = default;

    virtual void visitComposite(const CompositeNode<T>& composite) const = 0;
    virtual void visitLeaf(const LeafNode<T>& leaf) const = 0;
};

/**
 * @brief Helper visitor for printing the generic component hierarchy
 * @tparam T The data type stored in the components
 */
template<typename T>
class PrintVisitorBase final : public ComponentVisitorBase<T> {
public:
    explicit PrintVisitorBase(std::ostream& os = std::cout) : mOs(os), mIndent(0) {}

    void visitComposite(const CompositeNode<T>& composite) const override {
        printIndent();
        mOs << "[Composite] " << composite.name();
        if (composite.hasData()) {
            mOs << " (data: " << composite.data() << ")";
        }
        mOs << "\n";
        
        mIndent += 2;
        for (const auto& child : composite) {
            child->accept(*this);
        }
        mIndent -= 2;
    }

    void visitLeaf(const LeafNode<T>& leaf) const override {
        printIndent();
        mOs << "[Leaf] " << leaf.name();
        if (leaf.hasData()) {
            mOs << " (data: " << leaf.data() << ")";
        }
        mOs << "\n";
    }

private:
    void printIndent() const {
        for (int i = 0; i < mIndent; ++i) {
            mOs << " ";
        }
    }

    std::ostream& mOs;
    mutable int mIndent;
};

}  // namespace CwAPI3D::Composite

