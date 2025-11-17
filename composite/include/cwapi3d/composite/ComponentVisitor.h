#pragma once

#include <cwapi3d/composite/composite_export.h>
#include <iostream>

// Forward declare to avoid circular dependency in this section
namespace cwapi3d::composite {
template<typename T> class ComponentBase;
template<typename T> class CompositeNode;
template<typename T> class LeafNode;
}

namespace cwapi3d::composite {

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

/**
 * @brief Visitor interface for traversing the component hierarchy
 */
class COMPOSITE_EXPORT ComponentVisitor {
   public:
    virtual ~ComponentVisitor() = default;

    virtual void visitComposite(const Composite& composite) const = 0;
    virtual void visitLeaf(const Leaf& leaf) const = 0;
};

/**
 * @brief Helper visitor for printing the component hierarchy
 */
class COMPOSITE_EXPORT PrintVisitor final : public ComponentVisitor {
   public:
    explicit PrintVisitor(std::ostream& os = std::cout) : mOs(os), mIndent(0) {}

    void visitComposite(const Composite& composite) const override;
    void visitLeaf(const Leaf& leaf) const override;

   private:
    std::ostream& mOs;
    int mIndent;
};

}  // namespace cwapi3d::composite

// ============================================================================
// Template Method Implementations
// Must include Component.h to get full class definitions
// ============================================================================

#include "cwapi3d/composite/Component.h"

namespace cwapi3d::composite {

// Accept method implementations for template classes
template<typename T>
void CompositeNode<T>::accept(const ComponentVisitorBase<T>& visitor) const {
    visitor.visitComposite(*this);
}

template<typename T>
void LeafNode<T>::accept(const ComponentVisitorBase<T>& visitor) const {
    visitor.visitLeaf(*this);
}

}  // namespace cwapi3d::composite
