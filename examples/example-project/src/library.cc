//
// Created by MichaelBrunner on 19/11/2025.
//

#include <iostream>
#include <map>
#include <memory>
#include <unordered_map>
#include <ranges>
#include <algorithm>
#include <functional>

#include <cwapi3d/CwAPI3D.h>

#include <cwapi3d/geometry/Point3D.h>
#include <cwapi3d/composite/Component.h>
#include <cwapi3d/utility/StringConversion.h>

struct ElementData
{
    CwAPI3D::elementID mElementID;
};

enum class ComponentType
{
    Composite,
    Leaf
};

template<typename T> class OutVisitorBase final: public CwAPI3D::Composite::ComponentVisitorBase<T>
{
    CwAPI3D::Interfaces::ICwAPI3DUtilityController *mUtilityController;

public:
    explicit OutVisitorBase(CwAPI3D::Interfaces::ICwAPI3DUtilityController *aUtilityController)
        : mUtilityController(aUtilityController)
    {
    }
    void visitComposite(const CwAPI3D::Composite::CompositeNode<T> &composite) const override
    {
        const std::wstring nameFormatted = std::format(L"[COMPOSITE] => {}: {}",
                                                       CwAPI3D::Utility::ToWString(composite.name()),
                                                       composite.data().mElementID);
        mUtilityController->printToConsole(nameFormatted.data());

        std::ranges::for_each(composite,
                              [this](const auto &child)
                              {
                                  child->accept(*this);
                              });
    }
    void visitLeaf(const CwAPI3D::Composite::LeafNode<T> &leaf) const override
    {
        const std::wstring nameFormatted = std::format(L"   [LEAF] => {}: {}",
                                                       CwAPI3D::Utility::ToWString(leaf.name()),
                                                       leaf.data().mElementID);
        mUtilityController->printToConsole(nameFormatted.data());
    }
    ~OutVisitorBase() override = default;
};

std::unordered_map<std::string, std::vector<CwAPI3D::elementID> > buildElementMap(CwAPI3D::ControllerFactory *factory)
{
    std::unordered_map<std::string, std::vector<CwAPI3D::elementID> > elementMap;

    const auto activeElementIds = factory->getElementController()->getActiveIdentifiableElementIDs();
    for (decltype(activeElementIds->count()) i = 0; std::cmp_less(i, activeElementIds->count()); ++i) {
        const auto subgroup = factory->getAttributeController()->getSubgroup(activeElementIds->at(i));
        elementMap[CwAPI3D::Utility::ToUtf8(subgroup->data())].push_back(activeElementIds->at(i));
    }

    return elementMap;
}

std::map<CwAPI3D::elementID, ComponentType> buildComponentType(CwAPI3D::ControllerFactory *factory)
{
    auto isComposite = [factory](const CwAPI3D::elementID elementId)
    {
        const auto elementType = factory->getAttributeController()->getElementType(elementId);
        std::vector<std::function<bool()> > functions;
        functions.push_back([elementType] { return elementType->isWall(); });
        functions.push_back([elementType] { return elementType->isFloor(); });
        functions.push_back([elementType] { return elementType->isRoof(); });
        functions.push_back([elementType] { return elementType->isContainer(); });
        return std::ranges::any_of(functions, [](const auto &aFunc) { return aFunc(); });
    };


    std::map<CwAPI3D::elementID, ComponentType> componentMap;

    const auto activeElementIds = factory->getElementController()->getActiveIdentifiableElementIDs();
    for (decltype(activeElementIds->count()) i = 0; std::cmp_less(i, activeElementIds->count()); ++i) {
        const auto type = isComposite(activeElementIds->at(i)) ? ComponentType::Composite : ComponentType::Leaf;
        componentMap[activeElementIds->at(i)] = type;
    }

    return componentMap;
}

CWAPI3D_PLUGIN bool plugin_x64_init(CwAPI3D::ControllerFactory *factory)
{
    const CwAPI3D::Geometry::Point3D point(1, 2, 3);
    std::cout << point.x();


    const auto elementMap = buildElementMap(factory);
    const auto componentMap = buildComponentType(factory);

    std::vector<std::unique_ptr<CwAPI3D::Composite::CompositeNode<ElementData> > > nodes;

    for (const auto &elementIds : elementMap | std::views::values) {
        const auto isComposite = std::ranges::find_if(elementIds,
                                                      [&componentMap](const auto elementId)
                                                      {
                                                          const auto lIt = componentMap.find(elementId);
                                                          return lIt != componentMap.end() && lIt->second ==
                                                              ComponentType::Composite;
                                                      });
        std::unique_ptr<CwAPI3D::Composite::CompositeNode<ElementData> > assemblyNode;
        if (isComposite != elementIds.end()) {
            assemblyNode = std::make_unique<CwAPI3D::Composite::CompositeNode<ElementData> >(
                "Composite",
                ElementData{*isComposite});
        }
        else {
            assemblyNode = std::make_unique<CwAPI3D::Composite::CompositeNode<ElementData> >(
                "Orphaned Composite",
                ElementData{0});
        }
        std::ranges::for_each(elementIds,
                              [&assemblyNode](const auto elementId)
                              {
                                  assemblyNode->addChild(
                                      std::make_unique<CwAPI3D::Composite::LeafNode<ElementData> >(
                                          "Leaf",
                                          ElementData{elementId}));
                              });
        nodes.push_back(std::move(assemblyNode));
    }

    std::ranges::for_each(nodes,
                          [factory](const auto &node)
                          {
                              node->accept(OutVisitorBase<ElementData>(factory->getUtilityController()));
                          });

    return true;
}
