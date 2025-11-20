#pragma once

#include <algorithm>
#include <functional>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

namespace CwAPI3D::Composite {

template <typename T>
class ComponentVisitorBase;

/**
 * @brief Abstract base class for all components in the generic composite pattern
 * @tparam T The data type that this component will hold
 */
template <typename T>
class ComponentBase {
   public:
    explicit ComponentBase(std::string name) : mName(std::move(name)) {}
    explicit ComponentBase(std::string name, T data)
        : mName(std::move(name)), mData(std::move(data)) {}
    virtual ~ComponentBase() = default;

    ComponentBase(const ComponentBase&) = delete;
    ComponentBase& operator=(const ComponentBase&) = delete;
    ComponentBase(ComponentBase&&) = default;
    ComponentBase& operator=(ComponentBase&&) = default;

    [[nodiscard]] virtual bool isComposite() const noexcept { return false; }
    [[nodiscard]] const std::string& name() const noexcept { return mName; }
    void setName(std::string name) { mName = std::move(name); }

    // Data accessors
    [[nodiscard]] bool hasData() const noexcept { return mData.has_value(); }
    [[nodiscard]] const T& data() const {
        if (!mData.has_value()) {
            throw std::runtime_error("Component has no data");
        }
        return mData.value();
    }

    void clearData() { mData.reset(); }

    [[nodiscard]] ComponentBase* parent() const noexcept { return mParent; }
    void setParent(ComponentBase* parent) noexcept { mParent = parent; }

    virtual void accept(const class ComponentVisitorBase<T>& visitor) const = 0;

   private:
    std::string mName;
    std::optional<T> mData;
    ComponentBase* mParent = nullptr;
};

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4251)  // Suppress DLL interface warnings for STL members
#endif

/**
 * @brief Composite node that can contain child components
 * @tparam T The data type that this component will hold
 */
template <typename T>
class CompositeNode final : public ComponentBase<T> {
   public:
    explicit CompositeNode(std::string name) : ComponentBase<T>(std::move(name)) {}
    explicit CompositeNode(std::string name, T data)
        : ComponentBase<T>(std::move(name), std::move(data)) {}
    ~CompositeNode() override = default;

    [[nodiscard]] bool isComposite() const noexcept override { return true; }

    void addChild(std::unique_ptr<ComponentBase<T>> child) {
        if (!child) {
            throw std::invalid_argument("Cannot add null child");
        }

        if (child.get() == this) {
            throw std::invalid_argument("Cannot add component as its own child");
        }

        // ancestor (would create cycle)
        const ComponentBase<T>* ancestor = this->parent();
        while (ancestor) {
            if (ancestor == child.get()) {
                throw std::invalid_argument("Cannot add ancestor as child (would create cycle)");
            }
            ancestor = ancestor->parent();
        }

        child->setParent(this);
        mChildren.push_back(std::move(child));
    }

    std::unique_ptr<ComponentBase<T>> removeChild(ComponentBase<T>* child) {
        if (!child) {
            return nullptr;
        }

        auto it = std::ranges::find_if(
            mChildren,
            [child](const std::unique_ptr<ComponentBase<T>>& c) { return c.get() == child; });

        if (it == mChildren.end()) {
            return nullptr;
        }

        auto removed = std::move(*it);
        removed->setParent(nullptr);
        mChildren.erase(it);

        return removed;
    }

    std::unique_ptr<ComponentBase<T>> removeChild(const std::string& name) {
        auto it = std::ranges::find_if(
            mChildren,
            [&name](const std::unique_ptr<ComponentBase<T>>& c) { return c->name() == name; });

        if (it == mChildren.end()) {
            return nullptr;
        }

        auto removed = std::move(*it);
        removed->setParent(nullptr);
        mChildren.erase(it);

        return removed;
    }

    void clearChildren() {
        for (const auto& child : mChildren) {
            child->setParent(nullptr);
        }
        mChildren.clear();
    }

    [[nodiscard]] size_t childCount() const noexcept { return mChildren.size(); }

    [[nodiscard]] ComponentBase<T>* getChild(size_t index) const {
        if (index >= mChildren.size()) {
            throw std::out_of_range("Child index out of range");
        }
        return mChildren[index].get();
    }

    [[nodiscard]] ComponentBase<T>* findChild(const std::string& name) const {
        auto it = std::ranges::find_if(
            mChildren,
            [&name](const std::unique_ptr<ComponentBase<T>>& c) { return c->name() == name; });

        return it != mChildren.end() ? it->get() : nullptr;
    }

    using Iterator = std::vector<std::unique_ptr<ComponentBase<T>>>::iterator;
    using ConstIterator = std::vector<std::unique_ptr<ComponentBase<T>>>::const_iterator;

    // Makes composite iterable compiler calls composite.begin() and composite.end()
    [[nodiscard]] Iterator begin() noexcept { return mChildren.begin(); }
    [[nodiscard]] Iterator end() noexcept { return mChildren.end(); }
    [[nodiscard]] ConstIterator begin() const noexcept { return mChildren.begin(); }
    [[nodiscard]] ConstIterator end() const noexcept { return mChildren.end(); }
    [[nodiscard]] ConstIterator cbegin() const noexcept { return mChildren.cbegin(); }
    [[nodiscard]] ConstIterator cend() const noexcept { return mChildren.cend(); }

    void traverse(const std::function<void(ComponentBase<T>&)>& func) {
        func(*this);
        for (auto& child : mChildren) {
            if (child->isComposite()) {
                dynamic_cast<CompositeNode<T>*>(child.get())->traverse(func);
            } else {
                func(*child);
            }
        }
    }

    void traverse(const std::function<void(const ComponentBase<T>&)>& func) const {
        func(*this);
        for (const auto& child : mChildren) {
            if (child->isComposite()) {
                dynamic_cast<const CompositeNode<T>*>(child.get())->traverse(func);
            } else {
                func(*child);
            }
        }
    }

    void accept(const ComponentVisitorBase<T>& visitor) const override;

   private:
    std::vector<std::unique_ptr<ComponentBase<T>>> mChildren;
};

/**
 * @brief Leaf component (cannot have children)
 * @tparam T The data type that this component will hold
 */
template <typename T>
class LeafNode final : public ComponentBase<T> {
   public:
    explicit LeafNode(std::string name) : ComponentBase<T>(std::move(name)) {}
    explicit LeafNode(std::string name, T data)
        : ComponentBase<T>(std::move(name), std::move(data)) {}
    ~LeafNode() override = default;

    void accept(const ComponentVisitorBase<T>& visitor) const override;
};

#ifdef _MSC_VER
#pragma warning(pop)
#endif

// Inline accept implementations for template types to ensure they are
// instantiated in any translation unit that includes this header.

template <typename T>
void CompositeNode<T>::accept(const ComponentVisitorBase<T>& visitor) const {
    visitor.visitComposite(*this);
}

template <typename T>
void LeafNode<T>::accept(const ComponentVisitorBase<T>& visitor) const {
    visitor.visitLeaf(*this);
}

}  // namespace CwAPI3D::Composite
