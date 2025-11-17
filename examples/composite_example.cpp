#include <cwapi3d/composite/Component.h>
#include <cwapi3d/composite/ComponentVisitor.h>
#include <iostream>

using namespace cwapi3d::composite;

int main() {
    std::cout << "=== CwAPI3D.Ex Composite Pattern Example ===\n\n";
    
    // Create a building hierarchy
    std::cout << "Creating building hierarchy...\n\n";

    const auto building = std::make_unique<Composite>("Office Building");
    
    // First floor
    auto floor1 = std::make_unique<Composite>("Floor 1");
    floor1->addChild(std::make_unique<Leaf>("Room 101 - Reception"));
    floor1->addChild(std::make_unique<Leaf>("Room 102 - Conference Room"));
    floor1->addChild(std::make_unique<Leaf>("Room 103 - Storage"));
    
    // Second floor
    auto floor2 = std::make_unique<Composite>("Floor 2");
    
    auto westWing = std::make_unique<Composite>("West Wing");
    westWing->addChild(std::make_unique<Leaf>("Room 201 - Office"));
    westWing->addChild(std::make_unique<Leaf>("Room 202 - Office"));
    
    auto eastWing = std::make_unique<Composite>("East Wing");
    eastWing->addChild(std::make_unique<Leaf>("Room 203 - Office"));
    eastWing->addChild(std::make_unique<Leaf>("Room 204 - Meeting Room"));
    
    floor2->addChild(std::move(westWing));
    floor2->addChild(std::move(eastWing));
    
    // Third floor
    auto floor3 = std::make_unique<Composite>("Floor 3");
    floor3->addChild(std::make_unique<Leaf>("Room 301 - Server Room"));
    floor3->addChild(std::make_unique<Leaf>("Room 302 - IT Office"));
    
    // Add floors to building
    building->addChild(std::move(floor1));
    building->addChild(std::move(floor2));
    building->addChild(std::move(floor3));
    
    // Print the hierarchy using visitor
    std::cout << "Building Hierarchy:\n";
    std::cout << "===================\n";
    const PrintVisitor printer(std::cout);
    building->accept(printer);
    
    // Traverse and count components
    std::cout << "\nTraversing hierarchy...\n";
    int totalComponents = 0;
    int compositeCount = 0;
    int leafCount = 0;
    
    building->traverse([&](const Component& comp) {
        ++totalComponents;
        if (comp.isComposite()) {
            ++compositeCount;
        } else {
            ++leafCount;
        }
    });
    
    std::cout << "Total components: " << totalComponents << "\n";
    std::cout << "Composites: " << compositeCount << "\n";
    std::cout << "Leaves: " << leafCount << "\n\n";
    
    // Find a specific component
    std::cout << "Finding components...\n";
    
    Component* floor2Comp = building->findChild("Floor 2");
    if (floor2Comp && floor2Comp->isComposite()) {
        const Composite* floor2Composite = dynamic_cast<Composite*>(floor2Comp);
        std::cout << "Found Floor 2 with " << floor2Composite->childCount() << " children\n";

        if (const Component* westWingComp = floor2Composite->findChild("West Wing")) {
            std::cout << "Found " << westWingComp->name() << "\n";
        }
    }
    
    // Demonstrate removal
    std::cout << "\nRemoving a component...\n";
    if (const auto removed = building->removeChild("Floor 3")) {
        std::cout << "Removed: " << removed->name() << "\n";
        std::cout << "Building now has " << building->childCount() << " floors\n";
    }
    
    return 0;
}
